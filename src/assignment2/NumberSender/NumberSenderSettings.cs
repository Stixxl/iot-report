using System;
using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using NumberSender.Interfaces;

namespace NumberSender
{
    public class NumberSenderSettings
    {
        public readonly int Duration;
        public readonly int TimeBetweenSending;

        public NumberSenderSettings(
            int duration = 30000, 
            int timeBetweenSending = 1000)
        {
            this.Duration = duration;
            this.TimeBetweenSending = timeBetweenSending;
        }
    }
}