using System;
using System.Threading;
using MQTTnet;
using MQTTnet.Client;
using MQTTnet.Client.Options;
using NumberSender.Interfaces;

namespace NumberSender
{
    public class EventPublisher : IEventPublisher
    {
        private readonly EventPublisherSettings _settings;

        private IMqttClient mqttClient { get; set; }

        public EventPublisher(EventPublisherSettings settings)
        {
            this._settings = settings;
            Console.WriteLine($"Initializing EventPublisher with ID {settings.ClientId}, URL {settings.Host} and Topic {settings.TopicName}");
        }

        public async void SendAsync(int data)
        {
            Console.WriteLine($"Sending Number {data} to MQTT Broker");
            var message = new MqttApplicationMessageBuilder()
                .WithTopic(this._settings.TopicName)
                .WithPayload(data.ToString())
                .Build();

            await this.mqttClient.PublishAsync(message, CancellationToken.None);
        }

        public async void ConnectAsync(){
                var options = new MqttClientOptionsBuilder()
                .WithClientId(_settings.ClientId)
                .WithTcpServer(_settings.Host)
                .Build();

            var factory = new MqttFactory();
            this.mqttClient = factory.CreateMqttClient();
            await this.mqttClient.ConnectAsync(options);
        }
    }
}