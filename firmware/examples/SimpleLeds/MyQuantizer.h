#ifndef MyQuantizer_h
#define MyQuantizer_h

#include "AudioStream.h"

class MyQuantizer : public AudioStream {
public:
  MyQuantizer();
  void update(void);

private:
  audio_block_t *inputQueueArray[1];
};

inline MyQuantizer::MyQuantizer()
  : AudioStream(1, inputQueueArray) {
}

inline void MyQuantizer::update(void) {
  // Receive input data
  audio_block_t *block = receiveReadOnly(0);
  audio_block_t *outputBlock = allocate();

  if (block && outputBlock) {
    for (uint8_t i = 0; i < AUDIO_BLOCK_SAMPLES; i++) {
      uint16_t offsetData = block->data[i] + 32768;
      uint8_t rounded = (float)offsetData / (float)65536 * 12 * 5;
      outputBlock->data[i] = (rounded  * (65536 / (12 * 5))) - 32768;
      // Serial.println(outputBlock->data[i]);
    }

    transmit(outputBlock);
  }

  if (block) {
    release(block);
  }

  if (outputBlock) {
    release(outputBlock);
  }
}

#endif
