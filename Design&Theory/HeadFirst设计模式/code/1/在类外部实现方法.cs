using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;

namespace ConsoleApplication6
{
    public interface IQuack
    {
        void quack();
    }
    public class CQuackWang : IQuack
    {
        public void quack()
        {
            Console.WriteLine("wangwang");
        }
    }
    public class CQuackMiao : IQuack
    {
        public void quack()
        {
            Console.WriteLine("miaomiao");
        }
    }
    abstract class Animal
    {
        abstract public void makeSound();
    }
    class Dog : Animal
    {
        IQuack iquack;
        public Dog()
        {
            iquack = new CQuackWang();
        }
        public override void makeSound()
        {
            iquack.quack();
        }
    }
    class Cat : Animal
    {
        IQuack iquack;
        public Cat()
        {
            iquack = new CQuackMiao();
        }
        public override void makeSound()
        {
            iquack.quack();
        }
    }
    class Program
    {
        static void Main(string[] args)
        {
            Animal animal = new Dog(),
                animal2 = new Cat();
            animal.makeSound();
            animal2.makeSound();
            Console.ReadKey();
        }
    }
}
