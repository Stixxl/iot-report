using Microsoft.Extensions.DependencyInjection;
using Microsoft.Extensions.Logging;
using Microsoft.Extensions.Logging.Console;
using NumberSender.Interfaces;

namespace NumberSender
{
    public class Program
    {
        public static void Main(string[] args)
        {
            var serviceProvider = new ServiceCollection()
                .AddLogging()
                .AddSingleton<NumberSenderSettings>(new NumberSenderSettings())
                .AddSingleton<EventPublisherSettings>(new EventPublisherSettings())
                .AddSingleton<INumberGenerator, NumberGenerator>()
                .AddSingleton<IEventPublisher, EventPublisher>()
                .AddSingleton<INumberSenderService, NumberSenderService>()
                .BuildServiceProvider();

            var numberSenderService = serviceProvider.GetService<INumberSenderService>();
            numberSenderService.run();
        }
    }
}