#include "kafka_producer.h"
#include <string>

using namespace kafka_helpers;

KafkaProducer::KafkaProducer(
    components::util::AtomicQueue<std::string> &producerQueue,
    kafka::Properties props, std::atomic<bool> &stopProducingFlag)
    : m_producerQueue(producerQueue), m_stopProducingFlag(stopProducingFlag),
      m_props(props) {}

void KafkaProducer::startProducing(const std::string &topic) {
  m_producerThread =
      std::thread(&KafkaProducer::producerThread, this, topic);
}
void KafkaProducer::producerThread(const std::string &topic) {
  try {
    // Create a producer instance.
    kafka::clients::KafkaProducer producer(m_props);

    // Read messages from stdin and produce to the broker.

    while (!m_stopProducingFlag) {
      if (!m_producerQueue.empty()) {
        std::string item = m_producerQueue.front();
        m_producerQueue.pop();
        auto record = kafka::clients::producer::ProducerRecord(
            topic, kafka::NullKey, kafka::Value(item.c_str(), item.size()));
        // Send the message.
        producer.send(
            record,
            // The delivery report handler
            [](const kafka::clients::producer::RecordMetadata &metadata,
               const kafka::Error &error) {
              if (!error) {
                std::cout << "% Message delivered: " << metadata.toString()
                          << std::endl;
              } else {
                std::cerr << "% Message delivery failed: " << error.message()
                          << std::endl;
              }
            },
            // The memory block given by record.value() would be copied
            kafka::clients::KafkaProducer::SendOption::ToCopyRecordValue);
      }
    }

    // producer.close(); // No explicit close is needed, RAII will take care of
    // it

  } catch (const kafka::KafkaException &e) {
    std::cerr << "% Unexpected exception caught: " << e.what() << std::endl;
  }
}