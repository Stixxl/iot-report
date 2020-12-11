using System;
using System.IO;
using Microsoft.Extensions.Configuration;
using Microsoft.Extensions.DependencyInjection;
using NumberSender.Interfaces;

namespace NumberSender
{
    public class Program
    {
        public static IConfigurationRoot configuration;
        
        public static void Main(string[] args)
        {

            ServiceCollection serviceCollection = new ServiceCollection();
            var serviceProvider = ConfigureServices(serviceCollection);

            var numberSenderService = serviceProvider.GetService<INumberSenderService>();
            numberSenderService.run();
        }
     
        private static ServiceProvider ConfigureServices(IServiceCollection serviceCollection)
        {

            // Build configuration
            configuration = new ConfigurationBuilder()
                .SetBasePath(Directory.GetParent(AppContext.BaseDirectory).FullName)
                .AddJsonFile("appsettings.json", false)
                .AddEnvironmentVariables()
                .Build();

            // Add access to generic IConfigurationRoot

            var duration = configuration.GetValue<int>("NumberSenderSettings:Duration");
            var tbs = configuration.GetValue<int>("NumberSenderSettings:TimeBetweenSending");

            var numberSenderSettings = new NumberSenderSettings(duration,tbs);

            var clientId = configuration.GetValue<string>("EventPublisherSettings:ClientId");
            var host = configuration.GetValue<string>("EventPublisherSettings:Host");
            var topicName = configuration.GetValue<string>("EventPublisherSettings:TopicName");

            var eventPublisherSettings = new EventPublisherSettings(clientId,host,topicName);

            serviceCollection.AddSingleton<IConfigurationRoot>(configuration);
            serviceCollection.AddLogging();
            serviceCollection.AddSingleton<NumberSenderSettings>(numberSenderSettings);
            serviceCollection.AddSingleton<EventPublisherSettings>(eventPublisherSettings);
            serviceCollection.AddSingleton<INumberGenerator, NumberGenerator>();
            serviceCollection.AddSingleton<IEventPublisher, EventPublisher>();
            serviceCollection.AddSingleton<INumberSenderService, NumberSenderService>();
            return serviceCollection.BuildServiceProvider();
        }
    }
}
