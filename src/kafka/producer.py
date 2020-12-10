from kafka import KafkaProducer
producer = KafkaProducer()
for i in range(100):
    producer.send('number', i)
