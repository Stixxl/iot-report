from kafka import KafkaConsumer, TopicPartition
import platform, socket, re, uuid, json, psutil, logging
import logging

logging.basicConfig(level=logging.DEBUG)

def getSystemInfo():
    try:
        info={}
        info['platform']=platform.system()
        info['platform-release']=platform.release()
        info['platform-version']=platform.version()
        info['architecture']=platform.machine()
        info['hostname']=socket.gethostname()
        info['ip-address']=socket.gethostbyname(socket.gethostname())
        info['mac-address']=':'.join(re.findall('..', '%012x' % uuid.getnode()))
        info['processor']=platform.processor()
        info['ram']=str(round(psutil.virtual_memory().total / (1024.0 ** 3)))+" GB"
        return json.dumps(info)
    except Exception as e:
        logging.exception(e)


print("Starting Consumer 1;")
print(json.loads(getSystemInfo()))

consumer = KafkaConsumer(bootstrap_servers="localhost:9093",
                         client_id="number_consumer1",
                         auto_offset_reset='earliest',
                         enable_auto_commit=False,
                         consumer_timeout_ms=1000)

partition = TopicPartition('number', 0)
consumer.assign([partition])
consumer.seek_to_beginning(partition)
print("All numbers found:")
numbers = []
for msg in consumer:
    numbers.append(int.from_bytes(msg.value, 'big'))
print(numbers)

