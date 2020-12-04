using System;
using System.Timers;
using NumberSender.Interfaces;

namespace NumberSender
{
    public class NumberSenderService : INumberSenderService
    {
        private static Timer sendTimer;

        private static Timer stopTimer;

        private readonly IEventPublisher _eventPublisher;
        private readonly INumberGenerator _numberGenerator;
        private readonly NumberSenderSettings _settings;

        public NumberSenderService(IEventPublisher eventPublisher, INumberGenerator numberGenerator,NumberSenderSettings settings)//
        {
            this._eventPublisher = eventPublisher;
            this._numberGenerator = numberGenerator;
            this._settings = settings;
        }

        public void run()
        {
            this._eventPublisher.ConnectAsync();

            SetupTimer(ref sendTimer, this._settings.TimeBetweenSending, SendEvent);
            SetupTimer(ref stopTimer, this._settings.Duration, StopSending);

            Console.WriteLine("Press the Enter key to exit the program at any time... ");
            Console.ReadLine();
        }

        private void SendEvent(Object source, System.Timers.ElapsedEventArgs e)
        {
            Console.WriteLine("The Elapsed event was raised at {0}", e.SignalTime);
            var numberToSend = this._numberGenerator.GetNumber();
            this._eventPublisher.SendAsync(numberToSend);
        }

        private void StopSending(Object source, System.Timers.ElapsedEventArgs e)
        {
            Console.WriteLine("Done.. all events are sent!", e.SignalTime);
            sendTimer.AutoReset = false;
            sendTimer.Enabled = false;
            Console.WriteLine("Press any key to stop the app", e.SignalTime);
        }

        private void SetupTimer(ref Timer timer, int intervalTime, System.Timers.ElapsedEventHandler e){
            timer = new System.Timers.Timer();
            timer.Interval = intervalTime;
            timer.Elapsed += e;
            timer.AutoReset = true;
            timer.Enabled = true;
        }
    }
}