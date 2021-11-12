https://github.com/morganstanley/modern-cpp-kafka
https://www.confluent.io/blog/modern-cpp-kafka-api-for-safe-easy-messaging/
https://dev.to/de_maric/what-is-a-consumer-group-in-kafka-49il
https://kafka.apache.org/documentation/#consumerconfigs
https://hadoop.apache.org/
./bin/zookeeper-server-start.sh config/zookeeper.properties
bin/kafka-server-start.sh config/server.properties

bin/kafka-console-producer.sh --topic transcode_jobs --bootstrap-server localhost:9092 
./bin/kafka-topics.sh --create --topic transcode_jobs --bootstrap-server localhost:9092 --partitions 1 --replication-factor 1