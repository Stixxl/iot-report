namespace NumberSender
{
    public class EventPublisherSettings
    {
        public readonly string ClientId;
        public readonly string Host;
        
        public EventPublisherSettings()
        {
            this.ClientId = "TestSender";
            this.Host = "127.0.0.1";
        }
    }
}