namespace NumberSender
{
    public class EventPublisherSettings
    {
        public readonly string ClientId;
        public readonly string Host;
        public readonly string TopicName;
        
        public EventPublisherSettings(
            string clientId = "DotNetClient", 
            string host = "127.0.0.1", 
            string topicName = "Test")
        {
            this.ClientId = clientId;
            this.Host = host;
            this.TopicName = topicName;
        }
    }
}