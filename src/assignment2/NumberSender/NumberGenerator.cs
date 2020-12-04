using System;
using NumberSender.Interfaces;

namespace NumberSender
{
    public class NumberGenerator : INumberGenerator
    {
        private readonly Random _random;
        public NumberGenerator()
        {
            this._random = new Random();
        }

        public int GetNumber()
        {
            return this._random.Next(1, 100);
        }
    }
}