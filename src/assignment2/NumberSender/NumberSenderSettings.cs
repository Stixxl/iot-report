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

        public NumberSenderSettings(int? duration = null, int? timeBetweenSending = null)
        {
            if (duration.HasValue){
                this.Duration = duration.Value;
            }
            else{
                this.Duration = 30000;
            }
            if (timeBetweenSending.HasValue){
                this.TimeBetweenSending = duration.Value;
            }
            else{
                this.TimeBetweenSending = 1000;
            }
        }
    }
}