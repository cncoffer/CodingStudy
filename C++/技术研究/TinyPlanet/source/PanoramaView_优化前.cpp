#include "stdafx.h"
#include "PanoramaView.h"
#include <FisheyeTransform.h>
#include <math.h>
#include <memory.h>
#include "PanoInsert.h"
#include "viocomm.h"

#include <opencv2\opencv.hpp>
#include <opencv2\imgproc.hpp>
#include <opencv2\core\core.hpp>
#include <math.h>
#include <vector>

#define PI 3.14159265358979323846

static int NATAN = 65536;
static int NSQRT = 65536;
static int NSQRT_SHIFT = 16;

static int MI_MULT = 4096;
static int MI_SHIFT = 12;

BOOL g_fnDoubleFisheyeRotateEye(PBIH pBihSrc, int nLEyeRotateState, int nREyeRotateState);

// number of subdivisions of the x-axis unity
static int UNIT_XSAMPLES = 256;
// number of subdivisions of the y-axis unity
static int UNIT_YSAMPLES = 1024;
// number of bits to shift to return to the 0-255 range
// corresponds to the division by (UNIT_YSAMPLES*UNIT_YSAMPLES)
static int SHIFT_Y = 20;

// maximum number of weights used to interpolate one pixel
static int MAX_WEIGHTS = 20;

// maximum value for the quality parameter
static int MAX_QUALITY = 6;

PanoramaView::PanoramaView(int* ai, int width, int height)
{
	quality = 1;

	pwidth = width;
	pheight = height;

	yaw = 0.0;
	hfov = 120;
	hfov_min = 30;
	hfov_max = 165;
	pitch = 0.0;
	pitch_max = 90;
	pitch_min = -90;
	yaw_max = 180;
	yaw_min = -180;

	pan_steps = 20;

	atan_LU_HR = NULL;
	atan_LU = NULL;
	dist_e = 1.0;
	PV_atan0_HR = 0;
	PV_pi_HR = 0;

	pdata = new int*[height];
	for (int i = 0; i < height; i++)
	{
		pdata[i] = ai;
		ai += width;
	}

	math_setLookUp();

	horizonPosition = 50;

	// check if the horizon is not in the middle of the image
	pitch_min_org = pitch_min;
	pitch_max_org = pitch_max;
	CheckHorizonPosition();

	if (hfov > yaw_max - yaw_min)
		hfov = yaw_max - yaw_min;
}

PanoramaView::~PanoramaView()
{
	delete pdata;
	math_dispose();
}

void PanoramaView::math_dispose()
{
	delete atan_LU_HR;
    delete atan_LU;
	delete sqrt_LU;
	atan_LU_HR = NULL;
    atan_LU = NULL;
	sqrt_LU = NULL;
}

void PanoramaView::math_setLookUp()
{
	if (pdata != NULL)
	{
		if (atan_LU_HR == NULL)
		{
			atan_LU_HR = new int[NATAN + 1];
			atan_LU = new double[NATAN + 1];
			sqrt_LU = new int[NSQRT + 1];

			double d1 = 1.0 / (double)NSQRT;
			double d = 0.0;
			for (int i = 0; i < NSQRT;)
			{
				sqrt_LU[i] = (int)(sqrt(1.0 + d * d) * NSQRT);
				i++;
				d += d1;
			}

			sqrt_LU[NSQRT] = (int)(sqrt(2.0) * NSQRT);

			d1 = 1.0 / (double)NATAN;
			d = 0.0;
			for (int j = 0; j < NATAN + 1;)
			{
				if (j < NATAN)
					atan_LU[j] = atan(d / (1.0 - d)) * 256;
				else
					atan_LU[j] = 402.12385965949352;
				j++;
				d += d1;
			}

		}
		math_updateLookUp(pwidth);
	}
}

void PanoramaView::math_updateLookUp(int i)
{
	int j = i << 6;
	if (PV_atan0_HR != j)
	{
		dist_e = (double)i / 6.2831853071795862;
		PV_atan0_HR = j;
		PV_pi_HR = 128 * i;
		for (int k = 0; k < NATAN + 1; k++)
			atan_LU_HR[k] = (int)(dist_e * atan_LU[k] + 0.5);

	}
}

void PanoramaView::CheckHorizonPosition()
{
	deltaYHorizonPosition = (100 - 2 * horizonPosition) * pheight / 100;
	if (pheight != pwidth >> 1)
	{
		double d = ((double)pheight / (double)pwidth) * 180;
		double deltaPitch = ((double)deltaYHorizonPosition / (double)pwidth) * 180;
		pitch_min = pitch_min_org;
		pitch_max = pitch_max_org;
		if (pitch_max > (d - deltaPitch))
			pitch_max = d - deltaPitch;
		if (pitch_min < (-d - deltaPitch))
			pitch_min = -d - deltaPitch;
	}
}

void PanoramaView::matrix_matrix_mult(double ad[][3], double ad1[][3], double ad2[][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ad2[i][j] = ad[i][0] * ad1[0][j] + ad[i][1] * ad1[1][j] + ad[i][2] * ad1[2][j];
		}
	}

}

void PanoramaView::SetMatrix(double d, double d1, double ad[][3], int i)
{
	double ad1[3][3];
	double ad2[3][3];
	ad1[0][0] = 1.0;
	ad1[0][1] = 0.0;
	ad1[0][2] = 0.0;
	ad1[1][0] = 0.0;
	ad1[1][1] = cos(d);
	ad1[1][2] = sin(d);
	ad1[2][0] = 0.0;
	ad1[2][1] = -ad1[1][2];
	ad1[2][2] = ad1[1][1];
	ad2[0][0] = cos(d1);
	ad2[0][1] = 0.0;
	ad2[0][2] = -sin(d1);
	ad2[1][0] = 0.0;
	ad2[1][1] = 1.0;
	ad2[1][2] = 0.0;
	ad2[2][0] = -ad2[0][2];
	ad2[2][1] = 0.0;
	ad2[2][2] = ad2[0][0];
	if (i == 1)
	{
		matrix_matrix_mult(ad1, ad2, ad);
		return;
	}
	else
	{
		matrix_matrix_mult(ad2, ad1, ad);
		return;
	}
}

void PanoramaView::math_set_int_matrix(double fov, double pan, double tilt, int vw)
{
	double a = (fov * 2 * 3.1415926535897931) / 360; // field of view in rad
	double p = (double)vw / (2 * tan(a / 2));
	SetMatrix((tilt * 2 * 3.1415926535897931) / 360, (pan * 2 * 3.1415926535897931) / 360, mt, 1);
	mt[0][0] /= p;
	mt[0][1] /= p;
	mt[0][2] /= p;
	mt[1][0] /= p;
	mt[1][1] /= p;
	mt[1][2] /= p;
	double ta =
		a <= 0.29999999999999999 ? 436906.66666666669 : 131072 / a;
	for (int j = 0; j < 3; j++)
	{
		for (int k = 0; k < 3; k++)
        {
			mi[j][k] = (long long)(ta * mt[j][k] * MI_MULT + 0.5);
        }
	}

}

double PanoramaView::math_fovy(double hFov, int vw, int vh)
{
	return (360.0 / PI)
		* atan(((double)vh / (double)vw)
		* tan(((hFov / 2) * PI) / 180));
}

int PanoramaView::PV_atan2_HR(int pi, int pj)
{
	long long i = pi;
	long long j = pj;
	int index;
	if (j > 0)
	if (i > 0)
		return atan_LU_HR[(int)((NATAN * i) / (j + i))];
	else
		return -atan_LU_HR[(int)((NATAN * -i) / (j - i))];
	if (j == 0)
	if (i > 0)
		return PV_atan0_HR;
	else
		return -PV_atan0_HR;
	if (i < 0)
	{
		index = (int)((NATAN * i) / (j + i));
		return atan_LU_HR[index] - PV_pi_HR;
	}
	else
		return -atan_LU_HR[(int)((NATAN * -i) / (j - i))] + PV_pi_HR;
}

int PanoramaView::PV_sqrt(int pi, int pj)
{
	long long i = pi;
	long long j = pj;
	if (i > j)
		return (int)(i * sqrt_LU[(int)((j << NSQRT_SHIFT) / i)] >> NSQRT_SHIFT);
	if (j == 0)
		return 0;
	else
		return (int)(j * sqrt_LU[(int)((i << NSQRT_SHIFT) / j)] >> NSQRT_SHIFT);
}

int PanoramaView::bil(int p00, int p01, int p10, int p11, int dx, int dy)
{
	int k1 = 255 - dx;
	int l1 = 255 - dy;
	int i2 = k1 * l1;
	int j2 = dy * k1;
	int k2 = dx * dy;
	int l2 = dx * l1;
	int i3 =
		i2 * (p00 >> 16 & 0xff)
		+ l2 * (p01 >> 16 & 0xff)
		+ j2 * (p10 >> 16 & 0xff)
		+ k2 * (p11 >> 16 & 0xff) & 0xff0000;
	int j3 =
		i2 * (p00 >> 8 & 0xff)
		+ l2 * (p01 >> 8 & 0xff)
		+ j2 * (p10 >> 8 & 0xff)
		+ k2 * (p11 >> 8 & 0xff) >> 16;
	int k3 =
		i2 * (p00 & 0xff)
		+ l2 * (p01 & 0xff)
		+ j2 * (p10 & 0xff)
		+ k2 * (p11 & 0xff) >> 16;
	return (int)(i3 + (j3 << 8) + k3 + 0xff000000);
}

void PanoramaView::math_transform(
	int** pd,
	int pw,
	int ph,
	int* v,
	int vw,
	int vh,
	bool bilinear,
	bool lanczos2)
{

	// flag: use nearest neighbour interpolation
	bool nn = (!bilinear && !lanczos2);

	bool firstTime;	// flag
	int itmp;	// temporary variable used as a loop index

	int mix = pw - 1;
	int miy = ph - deltaYHorizonPosition - 1;
	int w2 = vw - 1 >> 1;
	int h2 = vh >> 1;
	int sw2 = pw >> 1;
	int sh2 = ph >> 1;
	int x_min = -w2;
	int x_max = vw - w2;
	int y_min = -h2;
	int y_max = vh - h2;
	int cy = 0;

	int xs_org, ys_org;	// used for lanczos2 interpolation
	int l24 = 0;
	int* pd_0 = pd[0];
	int* pd_1 = pd[1];
	long long m0 = mi[1][0] * y_min + mi[2][0];
	long long m1 = mi[1][1] * y_min + mi[2][1];
	long long m2 = mi[1][2] * y_min + mi[2][2];
	long long mi_00 = mi[0][0];
	long long mi_02 = mi[0][2];
	double vfov_2 = math_fovy(hfov, vw, vh) / 2;

	// number of points to be computed with linear interpolation
	// between two correctly computed points along the x-axis
	int N_POINTS_INTERP_X = vw / 20;

	if (pitch + vfov_2 > 65 || pitch - vfov_2 < -65) N_POINTS_INTERP_X = vw / 35;
	if (pitch + vfov_2 > 70 || pitch - vfov_2 < -70) N_POINTS_INTERP_X = vw / 50;
	if (pitch + vfov_2 > 80 || pitch - vfov_2 < -80) N_POINTS_INTERP_X = vw / 200;
	int N_POINTS_INTERP_X_P1 = N_POINTS_INTERP_X + 1;

	// number of rows to be computed with linear interpolation
	// between two correctly computed rows along the y-axis
	int N_POINTS_INTERP_Y;
	int N_POINTS_INTERP_Y_P1;

	// first row to draw without y-interpolation (default none)
	int N_ROW_NO_INTERP_MIN = y_max + 100;
	// last row to draw without y-interpolation (default none)
	int N_ROW_NO_INTERP_MAX = N_ROW_NO_INTERP_MIN;

	// last row of the upper part of the window to draw with double y-interpolation (default none)
	// we will use double distance from row 0 to this row
	int N_ROW_DOUBLE_INTERP_LOW = y_min - 100;
	// first row of the lower part of the window to draw with double y-interpolation (default none)
	// we will use double distance from this row to the last one
	int N_ROW_DOUBLE_INTERP_HIGH = y_max + 100;

	if (vfov_2 > 10)
	{ // only if not zooming in too much...
		// we consider critical the zone at +/- 5 degrees from the poles
		if (pitch + vfov_2 > 87.5 || pitch - vfov_2 < -87.5)
		{
			if (pitch > 0)
			{
				// looking upwards
				N_ROW_NO_INTERP_MIN = y_min + (int)((y_max - y_min) *
					(1 - (92.5 - (pitch - vfov_2)) / (2 * vfov_2)));
				N_ROW_NO_INTERP_MAX = y_min + (int)((y_max - y_min) *
					(1 - (87.5 - (pitch - vfov_2)) / (2 * vfov_2)));
			}
			else
			{
				N_ROW_NO_INTERP_MIN = y_min + (int)((y_max - y_min) *
					(1 - (-87.5 - (pitch - vfov_2)) / (2 * vfov_2)));
				N_ROW_NO_INTERP_MAX = y_min + (int)((y_max - y_min) *
					(1 - (-92.5 - (pitch - vfov_2)) / (2 * vfov_2)));

			}
		}
		// we draw with double y-interpolation the zone outside +/- 10 degrees from the poles
		double angle = 10;
		if (pitch + vfov_2 > 90 - angle || pitch - vfov_2 < -90 + angle)
		{
			if (pitch > 0)
			{
				// looking upwards
				N_ROW_DOUBLE_INTERP_LOW = y_min + (int)((y_max - y_min) *
					(1 - (90 + angle - (pitch - vfov_2)) / (2 * vfov_2)));
				N_ROW_DOUBLE_INTERP_HIGH = y_min + (int)((y_max - y_min) *
					(1 - (90 - angle - (pitch - vfov_2)) / (2 * vfov_2)));
			}
			else
			{
				N_ROW_DOUBLE_INTERP_LOW = y_min + (int)((y_max - y_min) *
					(1 - (-90 + angle - (pitch - vfov_2)) / (2 * vfov_2)));
				N_ROW_DOUBLE_INTERP_HIGH = y_min + (int)((y_max - y_min) *
					(1 - (-90 - angle - (pitch - vfov_2)) / (2 * vfov_2)));

			}
		}
	}
	///////////////////////////////////////////////////////////

	// data used for interpolation between rows:
	// size of the arrays used to store row values
	int ROWS_INT_SIZE = vw / N_POINTS_INTERP_X + 4;	// just to be safe...
	// coordinates of vertices in the upper computed row
	int* row_xold = new int[ROWS_INT_SIZE];
	int* row_yold = new int[ROWS_INT_SIZE];
	// coordinates of vertices in the lower computed row
	int* row_xnew = new int[ROWS_INT_SIZE];
	int* row_ynew = new int[ROWS_INT_SIZE];
	// difference between each interpolated line
	int* row_xdelta = new int[ROWS_INT_SIZE];
	int* row_ydelta = new int[ROWS_INT_SIZE];
	// used when drawing a line, contains the interpolted values every N_POINTS_INTERP_P1 pixels
	int* row_xcurrent = new int[ROWS_INT_SIZE];
	int* row_ycurrent = new int[ROWS_INT_SIZE];

	// shifted widh of the panorama
	int pw_shifted = (pw << 8);
	int pw_shifted_2 = pw_shifted / 2;
	int pw_shifted_3 = pw_shifted / 3;

	// used for linear interpolation 
	int x_old;
	int y_old;

	firstTime = true;
	long long v0 = m0 + x_min * mi_00;
	long long v1 = m1;
	long long v2 = m2 + x_min * mi_02;

	N_POINTS_INTERP_Y = N_POINTS_INTERP_X;
	N_POINTS_INTERP_Y_P1 = N_POINTS_INTERP_Y + 1;
	int nPtsInterpXOrg = N_POINTS_INTERP_X;	// stores the original value for future reference

	for (int y = y_min; y < y_max;)
	{
		int idx = cy;

		// if we are drawing one of the poles we disable interpolation between rows
		// to avoid artifacts
		if ((y + N_POINTS_INTERP_Y_P1 > N_ROW_NO_INTERP_MIN) &&
			(y < N_ROW_NO_INTERP_MAX))
		{
			N_POINTS_INTERP_Y = 0;
			if (N_POINTS_INTERP_X != nPtsInterpXOrg)
			{
				N_POINTS_INTERP_X = nPtsInterpXOrg;
				firstTime = true;   // to recompute the arrays
			}
		}
		else
		{
			if ((y + N_POINTS_INTERP_Y_P1 < N_ROW_DOUBLE_INTERP_LOW) ||
				(y > N_ROW_DOUBLE_INTERP_HIGH))
			{
				// we are farther from the pole so we compute more rows with interpolation
				N_POINTS_INTERP_Y = nPtsInterpXOrg * 4;
				// since we are far from the poles we can interpolate between more pixels
				if (N_POINTS_INTERP_X != nPtsInterpXOrg * 4)
				{
					N_POINTS_INTERP_X = nPtsInterpXOrg * 4;
					firstTime = true;   // to recompute the arrays
				}
			}
			else
			{
				N_POINTS_INTERP_Y = N_POINTS_INTERP_X;
			}
		}
		N_POINTS_INTERP_Y_P1 = N_POINTS_INTERP_Y + 1;
		N_POINTS_INTERP_X_P1 = N_POINTS_INTERP_X;	

		if (!firstTime)
		{
			for (itmp = 0; itmp < ROWS_INT_SIZE; itmp++)
			{
				row_xold[itmp] = row_xnew[itmp];
				row_yold[itmp] = row_ynew[itmp];
			}
			m0 += mi[1][0] * N_POINTS_INTERP_Y_P1;
			m1 += mi[1][1] * N_POINTS_INTERP_Y_P1;
			m2 += mi[1][2] * N_POINTS_INTERP_Y_P1;
		}

		// computes row_new[]
		v0 = m0 + x_min * mi_00;
		v1 = m1;
		v2 = m2 + x_min * mi_02;
		int irow = 0;	  // index in the row_*[] arrays
		int curx = x_min;  // x position of the current pixel in the viewer window
		row_xnew[irow] = PV_atan2_HR((int)v0 >> MI_SHIFT, (int)v2 >> MI_SHIFT);
		row_ynew[irow] = PV_atan2_HR((int)v1 >> MI_SHIFT, PV_sqrt((int)abs((int)v2 >> MI_SHIFT), (int)abs((int)v0 >> MI_SHIFT)));

		while (curx <= x_max)
		{
			v0 += mi_00 * N_POINTS_INTERP_X_P1;
			v2 += mi_02 * N_POINTS_INTERP_X_P1;

			curx += N_POINTS_INTERP_X_P1;
			irow++;
			row_xnew[irow] = PV_atan2_HR((int)v0 >> MI_SHIFT, (int)v2 >> MI_SHIFT);
			row_ynew[irow] = PV_atan2_HR((int)v1 >> MI_SHIFT, PV_sqrt((int)abs((int)v2 >> MI_SHIFT), (int)abs((int)v0 >> MI_SHIFT)));
		}

		if (firstTime)
		{
			// the first time only computes the first row and loops: that computation should be done before the loop
			// but I didn't like the idea of duplicating so much code so I arranged the code in such a way
			firstTime = false;
			continue;
		}

		// computes row_delta[], the difference between each row
		for (itmp = 0; itmp < ROWS_INT_SIZE; itmp++)
		{
			if ((row_xnew[itmp] < -pw_shifted_3) && (row_xold[itmp] > pw_shifted_3))
				row_xdelta[itmp] =
				(row_xnew[itmp] + pw_shifted - row_xold[itmp]) / (N_POINTS_INTERP_Y_P1);
			else
			{
				if ((row_xnew[itmp] > pw_shifted_3) && (row_xold[itmp] < -pw_shifted_3))
					row_xdelta[itmp] =
					(row_xnew[itmp] - pw_shifted - row_xold[itmp]) / (N_POINTS_INTERP_Y_P1);
				else
					row_xdelta[itmp] = (row_xnew[itmp] - row_xold[itmp]) / (N_POINTS_INTERP_Y_P1);
			}
			row_ydelta[itmp] = (row_ynew[itmp] - row_yold[itmp]) / N_POINTS_INTERP_Y_P1;
		}

		// row_current[] contains the values for the current row
		for (itmp = 0; itmp < ROWS_INT_SIZE; itmp++)
		{
			row_xcurrent[itmp] = row_xold[itmp];
			row_ycurrent[itmp] = row_yold[itmp];
		}

		// now draws a set of lines
		for (int ky = 0; ky < N_POINTS_INTERP_Y_P1; ky++)
		{

			if (y >= y_max) break;

			irow = 0;
			x_old = row_xcurrent[irow];
			y_old = row_ycurrent[irow];

			for (int x = x_min + 1; x <= x_max;)
			{
				v0 += mi_00 * N_POINTS_INTERP_X_P1;
				v2 += mi_02 * N_POINTS_INTERP_X_P1;
				irow++;
				// determines the next point: it will interpolate between the new and old point
				int x_new = row_xcurrent[irow];
				int y_new = row_ycurrent[irow];

				int delta_x;
				if ((x_new < -pw_shifted_3) && (x_old > pw_shifted_3))
					delta_x =
					(x_new + pw_shifted - x_old) / (N_POINTS_INTERP_X_P1);
				else
				{
					if ((x_new > pw_shifted_3) && (x_old < -pw_shifted_3))
						delta_x =
						(x_new - pw_shifted - x_old) / (N_POINTS_INTERP_X_P1);
					else
						delta_x = (x_new - x_old) / (N_POINTS_INTERP_X_P1);
				}
				int delta_y = (y_new - y_old) / (N_POINTS_INTERP_X_P1);

				// now computes the intermediate points with linear interpolation
				int cur_x = x_old;
				int cur_y = y_old;
				for (int kk = 0; kk < N_POINTS_INTERP_X_P1; kk++)
				{
					if (x > x_max)
						break;
					if (cur_x >= pw_shifted_2)
						cur_x -= pw_shifted;
					if (cur_x < -pw_shifted_2)
						cur_x += pw_shifted;
					cur_y += delta_y;
					int dx = cur_x & 0xff;
					int dy = cur_y & 0xff;
					int xs = (cur_x >> 8) + sw2;
					int ys;
					int v_idx = v[idx];

					// used for nn interpolation
					ys_org = (cur_y >> 8) + sh2 - deltaYHorizonPosition;
					int* pd_row = NULL;
					int row_index, col_index;
					if (nn)
					{
						if (dy < 128)
							row_index = ys_org;
						else
							row_index = ys_org + 1;
						if (row_index < 0) row_index = 0;
						if (row_index > miy) row_index = miy;
						pd_row = pd[row_index];
					}
					if (v_idx == 0)
					{
						// draws the pixel
						xs_org = xs;
						if (v_idx == 0)
						{
							if (nn)
							{
								if (dx < 128)
									col_index = xs_org;
								else
									col_index = xs_org + 1;
								if (col_index < 0) col_index = 0;
								if (col_index > mix) col_index = mix;
								int pxl = pd_row[col_index];
								v[idx] = (int)(pxl | 0xff000000);
							}
							else
							{
								int px00;
								int px01;
								int px10;
								int px11;
								if ((ys = ys_org) == l24
									&& xs >= 0
									&& xs < mix)
								{
									px00 = pd_0[xs];
									px10 = pd_1[xs++];
									px01 = pd_0[xs];
									px11 = pd_1[xs];
								}
								else if (
									ys >= 0 && ys < miy && xs >= 0 && xs < mix)
								{
									l24 = ys;
									pd_0 = pd[ys];
									pd_1 = pd[ys + 1];
									px00 = pd_0[xs];
									px10 = pd_1[xs++];
									px01 = pd_0[xs];
									px11 = pd_1[xs];
								}
								else
								{
									if (ys < 0)
									{
										pd_0 = pd[0];
										l24 = 0;
									}
									else if (ys > miy)
									{
										pd_0 = pd[miy];
										l24 = miy;
									}
									else
									{
										pd_0 = pd[ys];
										l24 = ys;
									}
									if (++ys < 0)
										pd_1 = pd[0];
									else if (ys > miy)
										pd_1 = pd[miy];
									else
										pd_1 = pd[ys];
									if (xs < 0)
									{
										px00 = pd_0[mix];
										px10 = pd_1[mix];
									}
									else if (xs > mix)
									{
										px00 = pd_0[0];
										px10 = pd_1[0];
									}
									else
									{
										px00 = pd_0[xs];
										px10 = pd_1[xs];
									}
									if (++xs < 0)
									{
										px01 = pd_0[mix];
										px11 = pd_1[mix];
									}
									else if (xs > mix)
									{
										px01 = pd_0[0];
										px11 = pd_1[0];
									}
									else
									{
										px01 = pd_0[xs];
										px11 = pd_1[xs];
									}
								}
								v[idx] = bil(px00, px01, px10, px11, dx, dy);
							}
						}
					}
					idx++;
					x++;
					cur_x += delta_x;
				}
				x_old = x_new;
				y_old = y_new;
			}

			// computes the next line using interpolation at the rows level
			for (itmp = 0; itmp < ROWS_INT_SIZE; itmp++)
			{
				row_xcurrent[itmp] += row_xdelta[itmp];
				row_ycurrent[itmp] += row_ydelta[itmp];
			}

			y++;
			cy += vw;
		}
	}
	delete row_xold;
	delete row_yold;
	delete row_xnew;
	delete row_ynew;
	delete row_xdelta;
	delete row_ydelta;
	delete row_xcurrent;
	delete row_ycurrent;
}

void PanoramaView::math_extractview(
	int** pd,
	int pw,
	int ph,
	int* v,
	int vw,
	int vh,
	double fov,
	double pan,
	double tilt,
	bool bilinear,
	bool lanczos2)
{
	math_set_int_matrix(fov, pan, tilt, vw);
	math_transform(
		pd,
		pw,
		ph + deltaYHorizonPosition,
		v,
		vw,
		vh, //v.Length / vw,
		bilinear,
		lanczos2);
}

double PanoramaView::math_view2pano(
	int i,
	int j,
	int k,
	int l,
	int i1,
	int j1,
	double d,
	double d1,
	double d2)
{
	double d8 = (double)i1 / 6.2831853071795862;
	double d3 = (d2 * 2 * 3.1415926535897931) / 360;
	double d4 = (int)((double)k / (2 * tan(d3 / 2)) + 0.5);
	SetMatrix(
		(d1 * 2 * 3.1415926535897931) / 360,
		(d * 2 * 3.1415926535897931) / 360,
		mt,
		1);
	i -= k >> 1;
	j -= l >> 1;
	double d5 =
		mt[0][0] * (double)i + mt[1][0] * (double)j + mt[2][0] * d4;
	double d6 =
		mt[0][1] * (double)i + mt[1][1] * (double)j + mt[2][1] * d4;
	double d7 =
		mt[0][2] * (double)i + mt[1][2] * (double)j + mt[2][2] * d4;
	return d8 * atan2(d5, d7) + (double)i1 / 2;
}

void PanoramaView::ExtractView()
{
	memset(vdata, 0, 4 * vwidth * vheight);
	switch (quality)
	{
	case 0:
		math_extractview(pdata, pwidth, pheight, vdata, vwidth, vheight, hfov, yaw, pitch, false, false);
		break;
	case 1:
		math_extractview(pdata, pwidth, pheight, vdata, vwidth, vheight, hfov, yaw, pitch, true, false);
		break;
	case 2:
		math_extractview(pdata, pwidth, pheight, vdata, vwidth, vheight, hfov, yaw, pitch, false, true);
		break;
	default:
		break;
	}
}

void PanoramaView::SetView(int* v, int vw, int vh)
{
	vdata = v;
	vwidth = vw;
	vheight = vh;
}

void PanoramaView::GotoView(double pan, double tilt, double fov)
{
	// reduces fov if it is too large for the vertical extension of this pano
	while (math_fovy(fov, vwidth, vheight) > pitch_max - pitch_min) {
		fov /= 1.03;
	}

	do
	{
		if (pan == yaw && tilt == pitch && fov == hfov)
			return;
		for (; pan > 180; pan -= 360);
		for (; pan < -180; pan += 360);
		double f = math_fovy(fov, vwidth, vheight) / 2;
		if (tilt > pitch_max - f && pitch_max != 90)
			tilt = pitch_max - f;
		else if (tilt > pitch_max)
			tilt = pitch_max;
		else if (tilt < pitch_min + f && pitch_min != -90)
			tilt = pitch_min + f;
		else if (tilt < pitch_min)
			tilt = pitch_min;
		if (yaw_max != 180 || yaw_min != -180)
        {
			// check left edge
			double xl =
				math_view2pano(
				0,
				pitch <= 0.0 ? vheight - 1 : 0,
				vwidth,
				vheight,
				pwidth,
				pheight,
				pan,
				tilt,
				fov);
			double xr =
				math_view2pano(
				vwidth - 1,
				pitch <= 0.0 ? vheight - 1 : 0,
				vwidth,
				vheight,
				pwidth,
				pheight,
				pan,
				tilt,
				fov);
			if (math_view2pano(vwidth - 1,
				pitch <= 0.0 ? vheight - 1 : 0,
				vwidth,
				vheight,
				pwidth,
				pheight,
				pan,
				tilt,
				fov)
				- xl
		        > ((yaw_max - yaw_min) / 360) * (double)pwidth)
		        break;
		}

		if (2 * f <= pitch_max - pitch_min
			&& fov <= hfov_max
			&& fov >= hfov_min
			&& fov <= yaw_max - yaw_min
			&& tilt <= pitch_max
			&& tilt >= pitch_min
			&& pan <= yaw_max
			&& pan >= yaw_min
			&& (pan != yaw || tilt != pitch || fov != hfov)) {
			yaw = pan;
			pitch = tilt;
			hfov = fov;
		}
	} while (false);
}

void PanoramaView::ZoomIn()
{
	GotoView(yaw, pitch, hfov / 1.03);
}

void PanoramaView::ZoomOut()
{
	GotoView(yaw, pitch, hfov * 1.03);
}

void PanoramaView::PanUp()
{
	GotoView(yaw, pitch + hfov / pan_steps, hfov);
}

void PanoramaView::PanDown()
{
	GotoView(yaw, pitch - hfov / pan_steps, hfov);
}

void PanoramaView::PanLeft()
{
	GotoView(yaw - hfov / pan_steps, pitch, hfov);
}

void PanoramaView::PanRight()
{
	GotoView(yaw + hfov / pan_steps, pitch, hfov);
}

void PanoramaView::Pan(double h, double v)
{
	GotoView(yaw + h * hfov / pan_steps, pitch + v * hfov / pan_steps, hfov);
}

#include <WindowsX.h>
#include "viocomm.h"
#include "Vcvrt.h"

BOOL  fnConvertBitCount(PBIH pBihSrc, PPBIH ppBihDest, WORD wBitCount)
{
    PBIH    pBihDest = *ppBihDest;
    DWORD   dwSize;

    
    if (pBihDest)
        if ((pBihDest->biWidth != pBihSrc->biWidth) || (pBihDest->biHeight != pBihSrc->biHeight) || (pBihDest->biBitCount != wBitCount)) {
            GlobalFreePtr(pBihDest);
            *ppBihDest = pBihDest = NULL;
        }

    if (!pBihDest) {
        dwSize = sizeof(BIH) + pBihSrc->biHeight * ((pBihSrc->biWidth * wBitCount / 8 + 3L) & ~3L);
        if (wBitCount == 8)
            dwSize += 256 * sizeof(RGBQUAD);
        pBihDest = (PBIH) GlobalAllocPtr(GHND, dwSize);
        if (!pBihDest) {
            return FALSE;
        }
    
        *pBihDest = *pBihSrc;
        pBihDest->biBitCount = wBitCount;
        pBihDest->biSizeImage = pBihSrc->biHeight * ((pBihSrc->biWidth * wBitCount / 8 + 3L) & ~3L);
        pBihDest->biClrUsed = (wBitCount != 8) ? 0: 256;
        *ppBihDest = pBihDest;
    }

    if (pBihSrc->biBitCount == 32 && pBihDest->biBitCount == 24) {
        WORD    wPerLineOffsetSrc, wPerLineOffsetDest;
        PBYTE   pBitsSrc, pBitsDest;
        LONG    lHeight, lWidth;        

        wPerLineOffsetSrc = ((WORD) pBihSrc->biWidth * pBihSrc->biBitCount / 8 + 3L) & ~3L
                            - pBihSrc->biWidth * pBihSrc->biBitCount / 8;
        wPerLineOffsetDest = ((WORD) pBihDest->biWidth * pBihDest->biBitCount / 8 + 3L) & ~3L
                            - pBihDest->biWidth * pBihDest->biBitCount / 8;
        pBitsSrc = uv_BihToData(pBihSrc);
        pBitsDest = uv_BihToData(pBihDest);

        for (lHeight = 0; lHeight < pBihSrc->biHeight; lHeight++) {
            for (lWidth = 0; lWidth < pBihSrc->biWidth; lWidth++) {
                *pBitsDest++ = *pBitsSrc++;
                *pBitsDest++ = *pBitsSrc++;
                *pBitsDest++ = *pBitsSrc++;
                pBitsSrc++;
            }
            pBitsSrc += wPerLineOffsetSrc;
            pBitsDest += wPerLineOffsetDest;
        }
    }
    else {
        if (!vcConvert((PSTR)uv_BihToData(pBihSrc), (PBITMAPINFO)pBihSrc, (PSTR)uv_BihToData(pBihDest), (PBITMAPINFO)pBihDest))
            return FALSE;
    }

    return TRUE;
}

BOOL g_fnReadFieldOfView_32(PBIH pBih360, PPBIH ppBihFOV, double pan, double tilt, double fov)
{
	// convert source 360 dib to 32-bit for class PanoramaView
	PBIH pBihSrc32 = NULL;
	if (!fnConvertBitCount(pBih360, &pBihSrc32, 32))
		return FALSE;

	// new class PanoramaView
	PanoramaView pano((int*)uv_BihToData(pBihSrc32), pBihSrc32->biWidth, pBihSrc32->biHeight);

	// setup output buffer
    BIH    BihFmt;
    BihFmt = *pBihSrc32;
    BihFmt.biWidth = pBihSrc32->biWidth/2;
    BihFmt.biHeight = pBihSrc32->biHeight/2;
	BihFmt.biSizeImage = uv_BihToSize(&BihFmt);
    DWORD dwSize = uv_BihToTotalSize(&BihFmt);
	PBIH pBihDst32 = NULL;
    pBihDst32 = (PBIH) GlobalAllocPtr(GHND, dwSize);
    if (!pBihDst32) {
		GlobalFreePtr(pBihSrc32);
        return FALSE;
    }
    *pBihDst32 = BihFmt;

	pano.SetView((int*)uv_BihToData(pBihDst32), pBihDst32->biWidth, pBihDst32->biHeight);
	pano.GotoView(pan, tilt, fov);

	// extract filed of view
	pano.ExtractView();

	// convert destination dib to 24-bit
	if (!fnConvertBitCount(pBihDst32, ppBihFOV, 24))
	{
		GlobalFreePtr(pBihSrc32);
		GlobalFreePtr(pBihDst32);
		return FALSE;
	}

	GlobalFreePtr(pBihSrc32);
	GlobalFreePtr(pBihDst32);
	return TRUE;
}

PanoramaView2::PanoramaView2(int* ai, int width, int height)
{
	quality = 1;

	pwidth = width;
	pheight = height;

	yaw = 0.0;
	hfov = 120;
	hfov_min = 30;
	hfov_max = 165;
	pitch = 0.0;
	pitch_max = 90;
	pitch_min = -90;
	yaw_max = 180;
	yaw_min = -180;

	pan_steps = 20;

	atan_LU_HR = NULL;
	atan_LU = NULL;
	dist_e = 1.0;
	PV_atan0_HR = 0;
	PV_pi_HR = 0;

	// modified for RGB24 begin
	int nBytesPerLine = (width * 3 + 3) & ~3;
	BYTE* bai = (BYTE*)ai;
	pdata = new int*[height];
	for (int i = 0; i < height; i++)
	{
		pdata[i] = (int*)bai;
		bai += nBytesPerLine;
	}
	// modified for RGB24 end

	math_setLookUp();

	horizonPosition = 50;

	// check if the horizon is not in the middle of the image
	pitch_min_org = pitch_min;
	pitch_max_org = pitch_max;
	CheckHorizonPosition();

	if (hfov > yaw_max - yaw_min)
		hfov = yaw_max - yaw_min;
}

PanoramaView2::~PanoramaView2()
{
	delete pdata;
	math_dispose();
}

void PanoramaView2::math_dispose()
{
	delete atan_LU_HR;
    delete atan_LU;
	delete sqrt_LU;
	atan_LU_HR = NULL;
    atan_LU = NULL;
	sqrt_LU = NULL;
}

void PanoramaView2::math_setLookUp()
{
	if (pdata != NULL)
	{
		if (atan_LU_HR == NULL)
		{
			atan_LU_HR = new int[NATAN + 1];
			atan_LU = new double[NATAN + 1];
			sqrt_LU = new int[NSQRT + 1];

			double d1 = 1.0 / (double)NSQRT;
			double d = 0.0;
			for (int i = 0; i < NSQRT;)
			{
				sqrt_LU[i] = (int)(sqrt(1.0 + d * d) * NSQRT);
				i++;
				d += d1;
			}

			sqrt_LU[NSQRT] = (int)(sqrt(2.0) * NSQRT);

			d1 = 1.0 / (double)NATAN;
			d = 0.0;
			for (int j = 0; j < NATAN + 1;)
			{
				if (j < NATAN)
					atan_LU[j] = atan(d / (1.0 - d)) * 256;
				else
					atan_LU[j] = 402.12385965949352;
				j++;
				d += d1;
			}

		}
		math_updateLookUp(pwidth);
	}
}

void PanoramaView2::math_updateLookUp(int i)
{
	int j = i << 6;
	if (PV_atan0_HR != j)
	{
		dist_e = (double)i / 6.2831853071795862;
		PV_atan0_HR = j;
		PV_pi_HR = 128 * i;
		for (int k = 0; k < NATAN + 1; k++)
			atan_LU_HR[k] = (int)(dist_e * atan_LU[k] + 0.5);

	}
}

void PanoramaView2::CheckHorizonPosition()
{
	deltaYHorizonPosition = (100 - 2 * horizonPosition) * pheight / 100;
	if (pheight != pwidth >> 1)
	{
		double d = ((double)pheight / (double)pwidth) * 180;
		double deltaPitch = ((double)deltaYHorizonPosition / (double)pwidth) * 180;
		pitch_min = pitch_min_org;
		pitch_max = pitch_max_org;
		if (pitch_max > (d - deltaPitch))
			pitch_max = d - deltaPitch;
		if (pitch_min < (-d - deltaPitch))
			pitch_min = -d - deltaPitch;
	}
}

void PanoramaView2::matrix_matrix_mult(double ad[][3], double ad1[][3], double ad2[][3])
{
	for (int i = 0; i < 3; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			ad2[i][j] = ad[i][0] * ad1[0][j] + ad[i][1] * ad1[1][j] + ad[i][2] * ad1[2][j];
		}
	}

}

void PanoramaView2::SetMatrix(double d, double d1, double ad[][3], int i)
{
	double ad1[3][3];
	double ad2[3][3];
	ad1[0][0] = 1.0;
	ad1[0][1] = 0.0;
	ad1[0][2] = 0.0;
	ad1[1][0] = 0.0;
	ad1[1][1] = cos(d);
	ad1[1][2] = sin(d);
	ad1[2][0] = 0.0;
	ad1[2][1] = -ad1[1][2];
	ad1[2][2] = ad1[1][1];
	ad2[0][0] = cos(d1);
	ad2[0][1] = 0.0;
	ad2[0][2] = -sin(d1);
	ad2[1][0] = 0.0;
	ad2[1][1] = 1.0;
	ad2[1][2] = 0.0;
	ad2[2][0] = -ad2[0][2];
	ad2[2][1] = 0.0;
	ad2[2][2] = ad2[0][0];
	if (i == 1)
	{
		matrix_matrix_mult(ad1, ad2, ad);
		return;
	}
	else
	{
		matrix_matrix_mult(ad2, ad1, ad);
		return;
	}
}

void PanoramaView2::math_set_int_matrix(double fov, double pan, double tilt, int vw)
{
	double a = (fov * 2 * 3.1415926535897931) / 360; // field of view in rad
	double p = (double)vw / (2 * tan(a / 2));
	SetMatrix((tilt * 2 * 3.1415926535897931) / 360, (pan * 2 * 3.1415926535897931) / 360, mt, 1);
	mt[0][0] /= p;
	mt[0][1] /= p;
	mt[0][2] /= p;
	mt[1][0] /= p;
	mt[1][1] /= p;
	mt[1][2] /= p;
	double ta =
		a <= 0.29999999999999999 ? 436906.66666666669 : 131072 / a;
	for (int j = 0; j < 3; j++)
	{
		for (int k = 0; k < 3; k++)
        {
			mi[j][k] = (long long)(ta * mt[j][k] * MI_MULT + 0.5);
        }
	}

}

double PanoramaView2::math_fovy(double hFov, int vw, int vh)
{
	return (360.0 / PI)
		* atan(((double)vh / (double)vw)
		* tan(((hFov / 2) * PI) / 180));
}

int PanoramaView2::PV_atan2_HR(int pi, int pj)
{
	long long i = pi;
	long long j = pj;
	int index;
	if (j > 0)
	if (i > 0)
		return atan_LU_HR[(int)((NATAN * i) / (j + i))];
	else
		return -atan_LU_HR[(int)((NATAN * -i) / (j - i))];
	if (j == 0)
	if (i > 0)
		return PV_atan0_HR;
	else
		return -PV_atan0_HR;
	if (i < 0)
	{
		index = (int)((NATAN * i) / (j + i));
		return atan_LU_HR[index] - PV_pi_HR;
	}
	else
		return -atan_LU_HR[(int)((NATAN * -i) / (j - i))] + PV_pi_HR;
}

int PanoramaView2::PV_sqrt(int pi, int pj)
{
	long long i = pi;
	long long j = pj;
	if (i > j)
		return (int)(i * sqrt_LU[(int)((j << NSQRT_SHIFT) / i)] >> NSQRT_SHIFT);
	if (j == 0)
		return 0;
	else
		return (int)(j * sqrt_LU[(int)((i << NSQRT_SHIFT) / j)] >> NSQRT_SHIFT);
}

int PanoramaView2::bil(int p00, int p01, int p10, int p11, int dx, int dy)
{
	int k1 = 255 - dx;
	int l1 = 255 - dy;
	int i2 = k1 * l1;
	int j2 = dy * k1;
	int k2 = dx * dy;
	int l2 = dx * l1;
	int i3 =
		i2 * (p00 >> 16 & 0xff)
		+ l2 * (p01 >> 16 & 0xff)
		+ j2 * (p10 >> 16 & 0xff)
		+ k2 * (p11 >> 16 & 0xff) & 0xff0000;
	int j3 =
		i2 * (p00 >> 8 & 0xff)
		+ l2 * (p01 >> 8 & 0xff)
		+ j2 * (p10 >> 8 & 0xff)
		+ k2 * (p11 >> 8 & 0xff) >> 16;
	int k3 =
		i2 * (p00 & 0xff)
		+ l2 * (p01 & 0xff)
		+ j2 * (p10 & 0xff)
		+ k2 * (p11 & 0xff) >> 16;
	return (int)(i3 + (j3 << 8) + k3 + 0xff000000);
}

void PanoramaView2::math_transform(
	int** pd,
	int pw,
	int ph,
	int* v,
	int vw,
	int vh,
	bool bilinear,
	bool lanczos2)
{

	// flag: use nearest neighbour interpolation
	bool nn = (!bilinear && !lanczos2);

	bool firstTime;	// flag
	int itmp;	// temporary variable used as a loop index

	int mix = pw - 1;
	int miy = ph - deltaYHorizonPosition - 1;
	int w2 = vw - 1 >> 1;
	int h2 = vh >> 1;
	int sw2 = pw >> 1;
	int sh2 = ph >> 1;
	int x_min = -w2;
	int x_max = vw - w2;
	int y_min = -h2;
	int y_max = vh - h2;
	int cy = 0;

	int xs_org, ys_org;	// used for lanczos2 interpolation
	int l24 = 0;
	// modified for RGB24 begin
	BYTE* pd_0 = (BYTE*)pd[0];
	BYTE* pd_1 = (BYTE*)pd[1];
	long long m0 = mi[1][0] * y_min + mi[2][0];
	long long m1 = mi[1][1] * y_min + mi[2][1];
	long long m2 = mi[1][2] * y_min + mi[2][2];
	long long mi_00 = mi[0][0];
	long long mi_02 = mi[0][2];
	double vfov_2 = math_fovy(hfov, vw, vh) / 2;

	// number of points to be computed with linear interpolation
	// between two correctly computed points along the x-axis
	int N_POINTS_INTERP_X = vw / 20;

	if (pitch + vfov_2 > 65 || pitch - vfov_2 < -65) N_POINTS_INTERP_X = vw / 35;
	if (pitch + vfov_2 > 70 || pitch - vfov_2 < -70) N_POINTS_INTERP_X = vw / 50;
	if (pitch + vfov_2 > 80 || pitch - vfov_2 < -80) N_POINTS_INTERP_X = vw / 200;
	if (N_POINTS_INTERP_X == 0)
		N_POINTS_INTERP_X = 1;
	int N_POINTS_INTERP_X_P1 = N_POINTS_INTERP_X + 1;

	// number of rows to be computed with linear interpolation
	// between two correctly computed rows along the y-axis
	int N_POINTS_INTERP_Y;
	int N_POINTS_INTERP_Y_P1;

	// first row to draw without y-interpolation (default none)
	int N_ROW_NO_INTERP_MIN = y_max + 100;
	// last row to draw without y-interpolation (default none)
	int N_ROW_NO_INTERP_MAX = N_ROW_NO_INTERP_MIN;

	// last row of the upper part of the window to draw with double y-interpolation (default none)
	// we will use double distance from row 0 to this row
	int N_ROW_DOUBLE_INTERP_LOW = y_min - 100;
	// first row of the lower part of the window to draw with double y-interpolation (default none)
	// we will use double distance from this row to the last one
	int N_ROW_DOUBLE_INTERP_HIGH = y_max + 100;

	if (vfov_2 > 10)
	{ // only if not zooming in too much...
		// we consider critical the zone at +/- 5 degrees from the poles
		if (pitch + vfov_2 > 87.5 || pitch - vfov_2 < -87.5)
		{
			if (pitch > 0)
			{
				// looking upwards
				N_ROW_NO_INTERP_MIN = y_min + (int)((y_max - y_min) *
					(1 - (92.5 - (pitch - vfov_2)) / (2 * vfov_2)));
				N_ROW_NO_INTERP_MAX = y_min + (int)((y_max - y_min) *
					(1 - (87.5 - (pitch - vfov_2)) / (2 * vfov_2)));
			}
			else
			{
				N_ROW_NO_INTERP_MIN = y_min + (int)((y_max - y_min) *
					(1 - (-87.5 - (pitch - vfov_2)) / (2 * vfov_2)));
				N_ROW_NO_INTERP_MAX = y_min + (int)((y_max - y_min) *
					(1 - (-92.5 - (pitch - vfov_2)) / (2 * vfov_2)));

			}
		}
		// we draw with double y-interpolation the zone outside +/- 10 degrees from the poles
		double angle = 10;
		if (pitch + vfov_2 > 90 - angle || pitch - vfov_2 < -90 + angle)
		{
			if (pitch > 0)
			{
				// looking upwards
				N_ROW_DOUBLE_INTERP_LOW = y_min + (int)((y_max - y_min) *
					(1 - (90 + angle - (pitch - vfov_2)) / (2 * vfov_2)));
				N_ROW_DOUBLE_INTERP_HIGH = y_min + (int)((y_max - y_min) *
					(1 - (90 - angle - (pitch - vfov_2)) / (2 * vfov_2)));
			}
			else
			{
				N_ROW_DOUBLE_INTERP_LOW = y_min + (int)((y_max - y_min) *
					(1 - (-90 + angle - (pitch - vfov_2)) / (2 * vfov_2)));
				N_ROW_DOUBLE_INTERP_HIGH = y_min + (int)((y_max - y_min) *
					(1 - (-90 - angle - (pitch - vfov_2)) / (2 * vfov_2)));

			}
		}
	}
	///////////////////////////////////////////////////////////

	// data used for interpolation between rows:
	// size of the arrays used to store row values
	int ROWS_INT_SIZE = vw / N_POINTS_INTERP_X + 4;	// just to be safe...
	// coordinates of vertices in the upper computed row
	int* row_xold = new int[ROWS_INT_SIZE];
	int* row_yold = new int[ROWS_INT_SIZE];
	// coordinates of vertices in the lower computed row
	int* row_xnew = new int[ROWS_INT_SIZE];
	int* row_ynew = new int[ROWS_INT_SIZE];
	// difference between each interpolated line
	int* row_xdelta = new int[ROWS_INT_SIZE];
	int* row_ydelta = new int[ROWS_INT_SIZE];
	// used when drawing a line, contains the interpolted values every N_POINTS_INTERP_P1 pixels
	int* row_xcurrent = new int[ROWS_INT_SIZE];
	int* row_ycurrent = new int[ROWS_INT_SIZE];

	// shifted widh of the panorama
	int pw_shifted = (pw << 8);
	int pw_shifted_2 = pw_shifted / 2;
	int pw_shifted_3 = pw_shifted / 3;

	// used for linear interpolation 
	int x_old;
	int y_old;

	firstTime = true;
	long long v0 = m0 + x_min * mi_00;
	long long v1 = m1;
	long long v2 = m2 + x_min * mi_02;

	N_POINTS_INTERP_Y = N_POINTS_INTERP_X;
	N_POINTS_INTERP_Y_P1 = N_POINTS_INTERP_Y + 1;
	int nPtsInterpXOrg = N_POINTS_INTERP_X;	// stores the original value for future reference

	BYTE* bv = (BYTE*)v;
	int nBytesPerLine = (vw * 3 + 3) & ~3;
	for (int y = y_min; y < y_max;)
	{
		int idx = 0;	//cy;

		// if we are drawing one of the poles we disable interpolation between rows
		// to avoid artifacts
		if ((y + N_POINTS_INTERP_Y_P1 > N_ROW_NO_INTERP_MIN) &&
			(y < N_ROW_NO_INTERP_MAX))
		{
			N_POINTS_INTERP_Y = 0;
			if (N_POINTS_INTERP_X != nPtsInterpXOrg)
			{
				N_POINTS_INTERP_X = nPtsInterpXOrg;
				firstTime = true;   // to recompute the arrays
			}
		}
		else
		{
			if ((y + N_POINTS_INTERP_Y_P1 < N_ROW_DOUBLE_INTERP_LOW) ||
				(y > N_ROW_DOUBLE_INTERP_HIGH))
			{
				// we are farther from the pole so we compute more rows with interpolation
				N_POINTS_INTERP_Y = nPtsInterpXOrg * 4;
				// since we are far from the poles we can interpolate between more pixels
				if (N_POINTS_INTERP_X != nPtsInterpXOrg * 4)
				{
					N_POINTS_INTERP_X = nPtsInterpXOrg * 4;
					firstTime = true;   // to recompute the arrays
				}
			}
			else
			{
				N_POINTS_INTERP_Y = N_POINTS_INTERP_X;
			}
		}
		N_POINTS_INTERP_Y_P1 = N_POINTS_INTERP_Y + 1;
		N_POINTS_INTERP_X_P1 = N_POINTS_INTERP_X;	

		if (!firstTime)
		{
			for (itmp = 0; itmp < ROWS_INT_SIZE; itmp++)
			{
				row_xold[itmp] = row_xnew[itmp];
				row_yold[itmp] = row_ynew[itmp];
			}
			m0 += mi[1][0] * N_POINTS_INTERP_Y_P1;
			m1 += mi[1][1] * N_POINTS_INTERP_Y_P1;
			m2 += mi[1][2] * N_POINTS_INTERP_Y_P1;
		}

		// computes row_new[]
		v0 = m0 + x_min * mi_00;
		v1 = m1;
		v2 = m2 + x_min * mi_02;
		int irow = 0;	  // index in the row_*[] arrays
		int curx = x_min;  // x position of the current pixel in the viewer window
		row_xnew[irow] = PV_atan2_HR((int)v0 >> MI_SHIFT, (int)v2 >> MI_SHIFT);
		row_ynew[irow] = PV_atan2_HR((int)v1 >> MI_SHIFT, PV_sqrt((int)abs((int)v2 >> MI_SHIFT), (int)abs((int)v0 >> MI_SHIFT)));

		while (curx <= x_max)
		{
			v0 += mi_00 * N_POINTS_INTERP_X_P1;
			v2 += mi_02 * N_POINTS_INTERP_X_P1;

			curx += N_POINTS_INTERP_X_P1;
			irow++;
			row_xnew[irow] = PV_atan2_HR((int)v0 >> MI_SHIFT, (int)v2 >> MI_SHIFT);
			row_ynew[irow] = PV_atan2_HR((int)v1 >> MI_SHIFT, PV_sqrt((int)abs((int)v2 >> MI_SHIFT), (int)abs((int)v0 >> MI_SHIFT)));
		}

		if (firstTime)
		{
			// the first time only computes the first row and loops: that computation should be done before the loop
			// but I didn't like the idea of duplicating so much code so I arranged the code in such a way
			firstTime = false;
			continue;
		}

		// computes row_delta[], the difference between each row
		for (itmp = 0; itmp < ROWS_INT_SIZE; itmp++)
		{
			if ((row_xnew[itmp] < -pw_shifted_3) && (row_xold[itmp] > pw_shifted_3))
				row_xdelta[itmp] =
				(row_xnew[itmp] + pw_shifted - row_xold[itmp]) / (N_POINTS_INTERP_Y_P1);
			else
			{
				if ((row_xnew[itmp] > pw_shifted_3) && (row_xold[itmp] < -pw_shifted_3))
					row_xdelta[itmp] =
					(row_xnew[itmp] - pw_shifted - row_xold[itmp]) / (N_POINTS_INTERP_Y_P1);
				else
					row_xdelta[itmp] = (row_xnew[itmp] - row_xold[itmp]) / (N_POINTS_INTERP_Y_P1);
			}
			row_ydelta[itmp] = (row_ynew[itmp] - row_yold[itmp]) / N_POINTS_INTERP_Y_P1;
		}

		// row_current[] contains the values for the current row
		for (itmp = 0; itmp < ROWS_INT_SIZE; itmp++)
		{
			row_xcurrent[itmp] = row_xold[itmp];
			row_ycurrent[itmp] = row_yold[itmp];
		}

		// now draws a set of lines
		for (int ky = 0; ky < N_POINTS_INTERP_Y_P1; ky++)
		{
			idx = 0;

			if (y >= y_max) break;

			irow = 0;
			x_old = row_xcurrent[irow];
			y_old = row_ycurrent[irow];

			for (int x = x_min + 1; x <= x_max;)
			{
				v0 += mi_00 * N_POINTS_INTERP_X_P1;
				v2 += mi_02 * N_POINTS_INTERP_X_P1;
				irow++;
				// determines the next point: it will interpolate between the new and old point
				int x_new = row_xcurrent[irow];
				int y_new = row_ycurrent[irow];

				int delta_x;
				if ((x_new < -pw_shifted_3) && (x_old > pw_shifted_3))
					delta_x =
					(x_new + pw_shifted - x_old) / (N_POINTS_INTERP_X_P1);
				else
				{
					if ((x_new > pw_shifted_3) && (x_old < -pw_shifted_3))
						delta_x =
						(x_new - pw_shifted - x_old) / (N_POINTS_INTERP_X_P1);
					else
						delta_x = (x_new - x_old) / (N_POINTS_INTERP_X_P1);
				}
				int delta_y = (y_new - y_old) / (N_POINTS_INTERP_X_P1);

				// now computes the intermediate points with linear interpolation
				int cur_x = x_old;
				int cur_y = y_old;
				for (int kk = 0; kk < N_POINTS_INTERP_X_P1; kk++)
				{
					if (x > x_max)
						break;
					if (cur_x >= pw_shifted_2)
						cur_x -= pw_shifted;
					if (cur_x < -pw_shifted_2)
						cur_x += pw_shifted;
					cur_y += delta_y;
					int dx = cur_x & 0xff;
					int dy = cur_y & 0xff;
					int xs = (cur_x >> 8) + sw2;
					int ys;
					int v_idx = *((int *)(bv+idx*3)) & 0x00FFFFFF;	//v[idx];

					// used for nn interpolation
					ys_org = (cur_y >> 8) + sh2 - deltaYHorizonPosition;
					BYTE* pd_row = NULL;
					int row_index, col_index;
					if (nn)
					{
						if (dy < 128)
							row_index = ys_org;
						else
							row_index = ys_org + 1;
						if (row_index < 0) row_index = 0;
						if (row_index > miy) row_index = miy;
						pd_row = (BYTE*)pd[row_index];
					}
					if (v_idx == 0)
					{
						// draws the pixel
						xs_org = xs;
						if (v_idx == 0)
						{
							if (nn)
							{
								if (dx < 128)
									col_index = xs_org;
								else
									col_index = xs_org + 1;
								if (col_index < 0) col_index = 0;
								if (col_index > mix) col_index = mix;
								int pxl = *((int *)(pd_row+col_index*3));	//pd_row[col_index]
								//v[idx] = (int)(pxl | 0xff000000);
								*((int *)(bv+idx*3)) = pxl & 0x00FFFFFF;
							}
							else
							{
								int px00;
								int px01;
								int px10;
								int px11;
								if ((ys = ys_org) == l24
									&& xs >= 0
									&& xs < mix)
								{
									px00 = *((int *)(pd_0+xs*3));	//pd_0[xs];
									px10 = *((int *)(pd_1+xs++*3));	//pd_1[xs++];
									px01 = *((int *)(pd_0+xs*3));	//pd_0[xs];
									px11 = *((int *)(pd_1+xs*3));	//pd_1[xs];
								}
								else if (
									ys >= 0 && ys < miy && xs >= 0 && xs < mix)
								{
									l24 = ys;
									pd_0 = (BYTE*)pd[ys];
									pd_1 = (BYTE*)pd[ys + 1];
									px00 = *((int *)(pd_0+xs*3));	//pd_0[xs];
									px10 = *((int *)(pd_1+xs++*3));	//pd_1[xs++];
									px01 = *((int *)(pd_0+xs*3));	//pd_0[xs];
									px11 = *((int *)(pd_1+xs*3));	//pd_1[xs];
								}
								else
								{
									if (ys < 0)
									{
										pd_0 = (BYTE*)pd[0];
										l24 = 0;
									}
									else if (ys > miy)
									{
										pd_0 = (BYTE*)pd[miy];
										l24 = miy;
									}
									else
									{
										pd_0 = (BYTE*)pd[ys];
										l24 = ys;
									}
									if (++ys < 0)
										pd_1 = (BYTE*)pd[0];
									else if (ys > miy)
										pd_1 = (BYTE*)pd[miy];
									else
										pd_1 = (BYTE*)pd[ys];
									if (xs < 0)
									{
										px00 = *((int *)(pd_0+mix*3));	//pd_0[mix];
										px10 = *((int *)(pd_1+mix*3));	//pd_1[mix];
									}
									else if (xs > mix)
									{
										px00 = *((int *)(pd_0));	//pd_0[0];
										px10 = *((int *)(pd_1));	//pd_1[0];
									}
									else
									{
										px00 = *((int *)(pd_0+xs*3));	//pd_0[xs];
										px10 = *((int *)(pd_1+xs*3));	//pd_1[xs];
									}
									if (++xs < 0)
									{
										px01 = *((int *)(pd_0+mix*3));	//pd_0[mix];
										px11 = *((int *)(pd_1+mix*3));	//pd_1[mix];
									}
									else if (xs > mix)
									{
										px01 = *((int *)(pd_0));	//pd_0[0];
										px11 = *((int *)(pd_1));	//pd_1[0];
									}
									else
									{
										px01 = *((int *)(pd_0+xs*3));	//pd_0[xs];
										px11 = *((int *)(pd_1+xs*3));	//pd_1[xs];
									}
								}
								//v[idx] = bil(px00, px01, px10, px11, dx, dy);
								*((int *)(bv+idx*3)) = bil(px00, px01, px10, px11, dx, dy) & 0x00FFFFFF;
							}
						}
					}
					idx++;
					x++;
					cur_x += delta_x;
				}
				x_old = x_new;
				y_old = y_new;
			}

			// computes the next line using interpolation at the rows level
			for (itmp = 0; itmp < ROWS_INT_SIZE; itmp++)
			{
				row_xcurrent[itmp] += row_xdelta[itmp];
				row_ycurrent[itmp] += row_ydelta[itmp];
			}

			y++;
			//cy += vw;
			bv += nBytesPerLine;
		}
	}
	// modified for RGB24 end
	delete row_xold;
	delete row_yold;
	delete row_xnew;
	delete row_ynew;
	delete row_xdelta;
	delete row_ydelta;
	delete row_xcurrent;
	delete row_ycurrent;
}

void PanoramaView2::math_extractview(
	int** pd,
	int pw,
	int ph,
	int* v,
	int vw,
	int vh,
	double fov,
	double pan,
	double tilt,
	bool bilinear,
	bool lanczos2)
{
	math_set_int_matrix(fov, pan, tilt, vw);
	math_transform(
		pd,
		pw,
		ph + deltaYHorizonPosition,
		v,
		vw,
		vh, //v.Length / vw,
		bilinear,
		lanczos2);
}

double PanoramaView2::math_view2pano(
	int i,
	int j,
	int k,
	int l,
	int i1,
	int j1,
	double d,
	double d1,
	double d2)
{
	double d8 = (double)i1 / 6.2831853071795862;
	double d3 = (d2 * 2 * 3.1415926535897931) / 360;
	double d4 = (int)((double)k / (2 * tan(d3 / 2)) + 0.5);
	SetMatrix(
		(d1 * 2 * 3.1415926535897931) / 360,
		(d * 2 * 3.1415926535897931) / 360,
		mt,
		1);
	i -= k >> 1;
	j -= l >> 1;
	double d5 =
		mt[0][0] * (double)i + mt[1][0] * (double)j + mt[2][0] * d4;
	double d6 =
		mt[0][1] * (double)i + mt[1][1] * (double)j + mt[2][1] * d4;
	double d7 =
		mt[0][2] * (double)i + mt[1][2] * (double)j + mt[2][2] * d4;
	return d8 * atan2(d5, d7) + (double)i1 / 2;
}

void PanoramaView2::ExtractView()
{
	// modified for RGB24 begin
	int nBytesPerLine = (vwidth * 3 + 3) & ~3;
	memset(vdata, 0, nBytesPerLine * vheight);
	// modified for RGB24 end
	switch (quality)
	{
	case 0:
		math_extractview(pdata, pwidth, pheight, vdata, vwidth, vheight, hfov, yaw, pitch, false, false);
		break;
	case 1:
		math_extractview(pdata, pwidth, pheight, vdata, vwidth, vheight, hfov, yaw, pitch, true, false);
		break;
	case 2:
		math_extractview(pdata, pwidth, pheight, vdata, vwidth, vheight, hfov, yaw, pitch, false, true);
		break;
	default:
		break;
	}
}

void PanoramaView2::SetView(int* v, int vw, int vh)
{
	vdata = v;
	vwidth = vw;
	vheight = vh;
}

void PanoramaView2::GotoView(double pan, double tilt, double fov)
{
	// reduces fov if it is too large for the vertical extension of this pano
	while (math_fovy(fov, vwidth, vheight) > pitch_max - pitch_min) {
		fov /= 1.03;
	}

	do
	{
		if (pan == yaw && tilt == pitch && fov == hfov)
			return;
		for (; pan > 180; pan -= 360);
		for (; pan < -180; pan += 360);
		double f = math_fovy(fov, vwidth, vheight) / 2;
		if (tilt > pitch_max - f && pitch_max != 90)
			tilt = pitch_max - f;
		else if (tilt > pitch_max)
			tilt = pitch_max;
		else if (tilt < pitch_min + f && pitch_min != -90)
			tilt = pitch_min + f;
		else if (tilt < pitch_min)
			tilt = pitch_min;
		if (yaw_max != 180 || yaw_min != -180)
        {
			// check left edge
			double xl =
				math_view2pano(
				0,
				pitch <= 0.0 ? vheight - 1 : 0,
				vwidth,
				vheight,
				pwidth,
				pheight,
				pan,
				tilt,
				fov);
			double xr =
				math_view2pano(
				vwidth - 1,
				pitch <= 0.0 ? vheight - 1 : 0,
				vwidth,
				vheight,
				pwidth,
				pheight,
				pan,
				tilt,
				fov);
			if (math_view2pano(vwidth - 1,
				pitch <= 0.0 ? vheight - 1 : 0,
				vwidth,
				vheight,
				pwidth,
				pheight,
				pan,
				tilt,
				fov)
				- xl
		        > ((yaw_max - yaw_min) / 360) * (double)pwidth)
		        break;
		}

		if (2 * f <= pitch_max - pitch_min
			&& fov <= hfov_max
			&& fov >= hfov_min
			&& fov <= yaw_max - yaw_min
			&& tilt <= pitch_max
			&& tilt >= pitch_min
			&& pan <= yaw_max
			&& pan >= yaw_min
			&& (pan != yaw || tilt != pitch || fov != hfov)) {
			yaw = pan;
			pitch = tilt;
			hfov = fov;
		}
	} while (false);
}

void g_fnGetFieldOfViewSize(LONG nSrcWidth, LONG nSrcHeight, LONG& nFovWidth, LONG &nFovHeight, BOOL bUseOrigSize = FALSE)
{
	if(bUseOrigSize)
	{
		nFovWidth = nSrcWidth;
		nFovHeight = nSrcWidth*9/16;
		return;
	}

	if (nSrcWidth >= 720 && nSrcWidth == nSrcHeight*2) // normal 360 video
	{
		nFovWidth = nSrcWidth/2;
		nFovHeight = (nSrcWidth/2)*9/16;
	}
	else if (nSrcWidth >= 720)
	{
		nFovWidth = nSrcWidth/2;
		nFovHeight = (nSrcWidth/2)*9/16;
	}
	else // little frame size
	{
		nFovWidth = nSrcWidth;
		nFovHeight = nSrcWidth*9/16;
	}
}

BOOL g_fnInsertAs360(PBIH pBihSrc, PPBIH ppBih360, double pan, double tilt, double fov)
{
	// setup output buffer
	PBIH pBihFOV = *ppBih360;
	if (!pBihFOV)
	{
		BIH    BihFmt;
		BihFmt = *pBihSrc;
		BihFmt.biWidth = pBihSrc->biWidth;
		BihFmt.biHeight = pBihSrc->biHeight;

		BihFmt.biSizeImage = uv_BihToSize(&BihFmt);
		DWORD dwSize = uv_BihToTotalSize(&BihFmt);
		pBihFOV = (PBIH) GlobalAllocPtr(GHND, dwSize);
		if (!pBihFOV) {
			return FALSE;
		}
		*pBihFOV = BihFmt;
		*ppBih360 = pBihFOV;
	}

	Picture src, dst;
	src.width = pBihSrc->biWidth;
	src.height = pBihSrc->biHeight;
	src.bitCount = pBihSrc->biBitCount;
	src.dataSize = uv_BihToSize(pBihSrc);
	src.data = uv_BihToData(pBihSrc);
	src.hfov = fov;
	src.pitch = tilt;
	src.yaw = pan;
	src.roll = 0;

	memcpy(&dst, &src, sizeof(src));
	dst.data = uv_BihToData(*ppBih360);
	dst.dataSize = uv_BihToSize(*ppBih360);
	dst.hfov = 0;
	PanoInsert(&src, &dst);

	return TRUE;
}
BOOL g_fnReadFieldOfView(PBIH pBih360, PPBIH ppBihFOV, double pan, double tilt, double fov, BOOL bUseOrigSize = FALSE)
{
	// new class PanoramaView
	PanoramaView2 pano((int*)uv_BihToData(pBih360), pBih360->biWidth, pBih360->biHeight);

	// setup output buffer
	PBIH pBihFOV = *ppBihFOV;
	if (!pBihFOV)
	{
		BIH    BihFmt;
		BihFmt = *pBih360;
		g_fnGetFieldOfViewSize(pBih360->biWidth, pBih360->biHeight, BihFmt.biWidth, BihFmt.biHeight, bUseOrigSize);
		BihFmt.biSizeImage = uv_BihToSize(&BihFmt);
		DWORD dwSize = uv_BihToTotalSize(&BihFmt);
		pBihFOV = (PBIH) GlobalAllocPtr(GHND, dwSize);
		if (!pBihFOV) {
			return FALSE;
		}
		*pBihFOV = BihFmt;
		*ppBihFOV = pBihFOV;
	}

	pano.SetView((int*)uv_BihToData(pBihFOV), pBihFOV->biWidth, pBihFOV->biHeight);
	pano.GotoView(pan, tilt, fov);

	// extract filed of view
	pano.ExtractView();
	return TRUE;
}

BOOL g_fnSingleFisheyeToEquirectangula(PBIH pBihSrc,PPBIH  m_equirectangula ,double CenterX,double CenterY,double Radius,int Fov,int Rotate)
{
	FisheyePicture source,destination;
	
	PBIH mid = *m_equirectangula;
	if (!mid)
	{
		BIH    BihFmt;
		BihFmt = *pBihSrc;
		BihFmt.biWidth = pBihSrc->biWidth*2;
		BihFmt.biHeight = pBihSrc->biHeight;

		BihFmt.biSizeImage = uv_BihToSize(&BihFmt);
		DWORD dwSize = uv_BihToTotalSize(&BihFmt);
		mid = (PBIH) GlobalAllocPtr(GHND, dwSize);
		if (!mid) {
			return FALSE;
		}
		*mid = BihFmt;
		*m_equirectangula = mid;
	}
	
	source.Data = new unsigned char [pBihSrc->biWidth*pBihSrc->biHeight*4];

	for(int i=0,j=0;i<uv_BihToSize(pBihSrc);i++)
	{
		if((j+1)%4 == 0) source.Data[j++] = 0;
		source.Data[j++] = uv_BihToData(pBihSrc)[i];
	}

	source.Cols = pBihSrc->biWidth;
	source.Rows = pBihSrc->biHeight;
	source.DataSize = source.Cols*source.Rows*4;
	source.Fov = Fov;
	source.Rotate = Rotate;
	source.CenterX = source.Rows*CenterX;
	source.CenterY = source.Cols*CenterY;
	source.Radius = source.Rows*Radius;
	
	
	destination.Cols = source.Cols*2;
	destination.Rows = source.Rows;
	destination.Data = new unsigned char[destination.Cols*destination.Rows*4];
	destination.DataSize = source.DataSize;
	destination.Fov = source.Fov;
	destination.CenterX = source.CenterX;
	destination.CenterY = source.CenterY;
	destination.Radius = source.Radius;
	
	SingleFisheye2Equirectangular(&source,&destination);
	
	for(int i=0,j=0;i<uv_BihToSize(mid);i++)
	{
		uv_BihToData(mid)[i]=destination.Data[j++];
		if( ( j + 1 ) % 4 == 0) j++;
	}

	return TRUE;
}

BOOL g_fnDoubleFisheyeToEquirectangula(PBIH pBihSrc,PPBIH  m_equirectangula , double LCenterX,double LCenterY,double RCenterX,double RCenterY,double Radius,int Fov,int LEyeRotate/*=0*/,int REyeRotate/*=0*/)
{
	g_fnDoubleFisheyeRotateEye(pBihSrc, LEyeRotate, REyeRotate);
	FisheyePicture source,destination;
	
	//PBIH pBihFOV = *ppBihDst;

	PBIH mid = *m_equirectangula;
	if (!mid)
	{
		BIH    BihFmt;
		BihFmt = *pBihSrc;
		BihFmt.biWidth = pBihSrc->biWidth;
		BihFmt.biHeight = pBihSrc->biHeight;

		BihFmt.biSizeImage = uv_BihToSize(&BihFmt);
		DWORD dwSize = uv_BihToTotalSize(&BihFmt);
		mid = (PBIH) GlobalAllocPtr(GHND, dwSize);
		if (!mid) {
			return FALSE;
		}
		*mid = BihFmt;
		*m_equirectangula = mid;
	}
	
	source.Data = new unsigned char [pBihSrc->biWidth*pBihSrc->biHeight*4];

	for(int i=0,j=0;i<uv_BihToSize(pBihSrc);i++)
	{
		if((j+1)%4 == 0) source.Data[j++] = 0;
		source.Data[j++] = uv_BihToData(pBihSrc)[i];
	}

	source.Cols = pBihSrc->biWidth;
	source.Rows = pBihSrc->biHeight;
	source.DataSize = source.Cols*source.Rows*4;
	source.Fov = Fov;
	source.LCenterX = source.Rows*LCenterX;
	source.LCenterY = source.Cols*LCenterY;
	source.RCenterX = source.Rows*RCenterX;
	source.RCenterY = source.Cols*RCenterY;
	source.Radius = source.Rows*Radius;

	destination.Cols = source.Cols;
	destination.Rows = source.Rows;
	destination.Data = new unsigned char[destination.Cols*destination.Rows*4];
	destination.DataSize = source.DataSize;
	destination.Fov = source.Fov;
	destination.LCenterX = source.LCenterX;
	destination.LCenterY = source.LCenterY;
	destination.RCenterX = source.RCenterX;
	destination.RCenterY = source.RCenterY;
	destination.Radius = source.Radius;

	DualFisheye2Equirectangular(&source,&destination);

	for(int i=0,j=0;i<uv_BihToSize(mid);i++)
	{
		uv_BihToData(mid)[i]=destination.Data[j++];
		if( ( j + 1 ) % 4 == 0) j++;
	}

	return TRUE;
}

BOOL g_fnDoubleFisheyeRotateEye(PBIH pBihSrc, int nLEyeRotateState, int nREyeRotateState)
{
	int nWidth = pBihSrc->biWidth;
	int nHeight = pBihSrc->biHeight;
	cv::Mat src(nHeight, nWidth, CV_8UC3, uv_BihToData(pBihSrc));
	
	cv::Rect LEyeROI = cv::Rect(0, 0, src.cols / 2, src.rows);
	cv::Rect REyeROI = cv::Rect(src.cols / 2, 0, src.cols / 2, src.rows);

	auto RotateEye = [&src] (cv::Rect ROI, int nRotateState) {
		double angle = nRotateState * 90;
		if (!angle)
			return;
		cv::Mat srcROIEye = src(ROI);
		cv::Mat temp, dst;
		cv::Size EyeSize = srcROIEye.size();
		int len = cv::max(EyeSize.width, EyeSize.height);
		if (EyeSize.width != EyeSize.height)
			cv::resize(srcROIEye, temp, cv::Size(len, len), 0, 0, 1);
		else
			temp = srcROIEye;
		cv::Size dstSize(len, len);
		cv::Point2f center(len * 0.5, len * 0.5);
		cv::Mat rotMat = cv::getRotationMatrix2D(center, angle, 1.0);
		cv::warpAffine(temp, dst, rotMat, dstSize);
		cv::resize(dst, srcROIEye, srcROIEye.size(), 0, 0, 1);
	};
	RotateEye(LEyeROI, nLEyeRotateState);
	RotateEye(REyeROI, nREyeRotateState);

	return TRUE;
}

BOOL g_fnTinyPlanet(PBIH pBihSrc, PPBIH pResult, int nLat/*=0*/, int nLon/*=0*/, int nZoom/*1*/)
{
	static int sZoom = -1, sLon = -1, sLat = -1;
	static int sW = 0, sH = 0;
	static int nHasDoCalu = FALSE;

	cv::Mat map_x, map_y, map_r, temp1, temp2;
	cv::Mat map_c, map_c_sin, map_c_cos, map_lat, map_lon;
	double dSinPhi0, dCosPhi0;
	double phi0, lamda0;
	int R;
	static cv::Mat remap_y, remap_x;
	
	// init dst bitmap
	PBIH mid = *pResult;
	if (!mid)
	{
		BIH    BihFmt;
		BihFmt = *pBihSrc;
		int nLength = std::min(pBihSrc->biHeight, pBihSrc->biWidth);
		BihFmt.biWidth = nLength;
		BihFmt.biHeight = nLength;

		BihFmt.biSizeImage = uv_BihToSize(&BihFmt);
		DWORD dwSize = uv_BihToTotalSize(&BihFmt);
		mid = (PBIH) GlobalAllocPtr(GHND, dwSize);
		if (!mid) {
			return FALSE;
		}
		*mid = BihFmt;
		*pResult = mid;
	}

	cv::Mat src(pBihSrc->biHeight, pBihSrc->biWidth, CV_8UC3, uv_BihToData(pBihSrc));
	cv::Mat dst(mid->biHeight, mid->biWidth, CV_8UC3, uv_BihToData(mid));
	cv::Mat img, base_map_x, base_map_y;
	cv::resize(src, img, src.size(), 1, 1, cv::INTER_LANCZOS4);
	
	const int nZoomMin = 1, nZoomMax = 100;
	const int nLatMin = 0, nLatMax = 180;
	const int nLonMin = 0, nLonMax = 360;

	// opencv implement
	nZoom = std::max(nZoomMin, std::min(nZoom, nZoomMax));
	nLat = std::max(nLatMin, std::min(nLat, nLatMax));
	nLon = std::max(nLonMin, std::min(nLon, nLonMax));
	if (sZoom == nZoom && sLat == nLat && sLon == nLon 
		&& sW == src.size().width && sH == src.size().height && nHasDoCalu == TRUE)
	{
		goto do_remap; // if para not change, do remap directly
	}
	else
	{
		sZoom = nZoom;
		sLat = nLat;
		sLon = nLon ;
		sW = src.size().width;
		sH = src.size().height;
		nHasDoCalu = FALSE;
	}

	// calu map matrix
	base_map_x = cv::Mat::zeros(img.size(),CV_32FC1);
	base_map_y = cv::Mat::zeros(img.size(),CV_32FC1);

	int i, j;
	for (i = 0; i < sH; i++)
		for (j = 0; j < sW; j++)
			base_map_x.at<float>(i,j) = j;

	for (i = 0; i < sH; i++)
		for (j = 0; j < sW; j++)
			base_map_y.at<float>(i,j) = i;

	sZoom = std::max(1, std::min(nZoom, 100));
	R = sW / sZoom;
	phi0 = sLat * PI / 180 - PI / 2;
	lamda0 = sLon * PI / 180;

	map_x = base_map_x - sW / 2;
	map_y = base_map_y - sH / 2;
	cv::multiply(map_x, map_x, temp1);
	cv::multiply(map_y, map_y, temp2);

	cv::sqrt(temp1 + temp2, map_r);
	
	map_c_sin = cv::Mat::zeros(img.size(),CV_32FC1);
	map_c_cos = cv::Mat::zeros(img.size(),CV_32FC1);
	map_lat = cv::Mat::zeros(img.size(),CV_32FC1);
	map_lon = cv::Mat::zeros(img.size(),CV_32FC1);
	for (i = 0; i < sH; i++)
		for (j = 0; j < sW; j++)
		{
			float dElement = 2 * std::atan(map_r.at<float>(i,j) / 2 / R);
			map_c_sin.at<float>(i,j) = std::sin(dElement);
			map_c_cos.at<float>(i,j) = std::cos(dElement);
		}
	
	dSinPhi0 = std::sin(phi0);
	dCosPhi0 = std::cos(phi0);
	for (i = 0; i < sH; i++)
		for (j = 0; j < sW; j++)
		{
			map_lat.at<float>(i,j) =  PI / 2 + std::asin(
				map_c_cos.at<float>(i,j) * dSinPhi0 
				+ map_y.at<float>(i,j) * map_c_sin.at<float>(i,j)
				* dCosPhi0 / map_r.at<float>(i,j));
		}
		
	for (i = 0; i < sH; i++)
		for (j = 0; j < sW; j++)
		{
			float temp = lamda0 + std::atan2(
				map_x.at<float>(i,j) * map_c_sin.at<float>(i,j),
				map_r.at<float>(i,j) * dCosPhi0 * map_c_cos.at<float>(i,j)
				- map_y.at<float>(i,j) * dSinPhi0 * map_c_sin.at<float>(i,j));
			map_lon.at<float>(i,j) = std::fmod(temp + PI, 2 * PI);
		}

	remap_y = (sH - 1) * map_lat / PI;
	remap_x = (sW - 1) * map_lon / (2 * PI);
	nHasDoCalu = TRUE;

	// release
	map_x.release();
	map_y.release();
	map_r.release();
	temp1.release();
	temp2.release();
	map_c.release();
	map_c_sin.release();
	map_c_cos.release();
	map_lat.release();
	map_lon.release();
	base_map_x.release();
	base_map_y.release();

#if 0 // todo: Eigen implement
	int h = img.size().height, w = img.size().width;
	Eigen::MatrixXf matcol = Eigen::MatrixXf::Zero(h, 1);
	Eigen::MatrixXf matrow = Eigen::MatrixXf::Ones(1, w);
	int i, j;
	for (i = 0; i < h; i++)
		matcol(i, 0) = i;
	Eigen::MatrixXf mat_x = matcol*matrow;

	matcol = Eigen::MatrixXf::Ones(h, 1);
	matrow = Eigen::MatrixXf::Zero(1, w);
	for (i = 0; i < w; i++)
		matrow(0, i) = i;
	Eigen::MatrixXf mat_y = matcol*matrow;
	
	sZoom = std::max(1, std::min(nZoom, 100));
	R = w / sZoom;
	phi0 = sLat * PI / 180 - PI / 2;
	lamda0 = sLon * PI / 180;
	dSinPhi0 = std::sin(phi0);
	dCosPhi0 = std::cos(phi0);
	
	mat_x = mat_x.array() - sW / 2;
	mat_y = mat_y.array() - sH / 2;
	Eigen::MatrixXf mat_r = (mat_x.array().pow(2) + mat_y.array().pow(2)).pow(0.5);
	Eigen::MatrixXf mat_c = (mat_r.array() / 2 / R).atan() * 2;
	Eigen::MatrixXf mat_lat = (mat_c.array() * dSinPhi0 + mat_y.array() 
		* mat_c.array().sin() * dCosPhi0 / mat_r.array());
	mat_lat = mat_lat.array().asin() + PI / 2;

	Eigen::MatrixXf mat_lon = (mat_x.array() * mat_c.array().sin()) 
		/ (mat_r.array() * dCosPhi0 * mat_c.array().cos() 
		- mat_y.array() * dSinPhi0 * mat_c.array().sin());
	mat_lon = lamda0 + mat_lon.array().atan();
	mat_lon = mat_lon.unaryExpr([&](const int x) {return std::fmod(x + PI, 2 * PI); });
	
	mat_lat = (sH - 1) * mat_lat.array() / PI;
	mat_lon = (sW - 1) * mat_lon.array() / (2 * PI);

	remap_y = cv::Mat::zeros(img.size(),CV_32FC1);
	remap_x = cv::Mat::zeros(img.size(),CV_32FC1);

	cv::eigen2cv(mat_lat, remap_y);
	cv::eigen2cv(mat_lon, remap_x);
#endif

do_remap:
	cv::Mat result;
	cv::remap(img, result, remap_x, remap_y, cv::INTER_LINEAR);

	// output 1:1 bitmap
	int nLength = dst.size().width;
	CRect rc(CPoint(result.cols/2, result.rows/2), CPoint(result.cols/2, result.rows/2));
	rc.InflateRect(nLength/2, nLength/2);
	cv::Rect ROI(rc.left, rc.top, rc.Width(), rc.Height());
	
	cv::Mat temp = result(ROI);
	temp.copyTo(dst);
	
	img.release();
	result.release();

	return TRUE;
}
