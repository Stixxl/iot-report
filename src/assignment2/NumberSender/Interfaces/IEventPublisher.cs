namespace NumberSender.Interfaces
{
    public interface IEventPublisher
    {
        void SendAsync(int data);

        void ConnectAsync();
    }
}