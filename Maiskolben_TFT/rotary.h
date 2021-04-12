#ifndef __ROTARY_H__
#define __ROTARY_H__

#include <stdint.h>

class Rotary {
  public:

  Rotary(uint8_t a, uint8_t b) : 
    _encoderDirCount(0), _prevEncoderState(0), 
    _nextEncoderState(0), _nextEncoderStateCount(0), _a(a), _b(b) {
  }

  void encoder_poll() {
    static const int8_t transition[][4] = {
      {0,-1,1,0}, {1,0,0,-1}, {-1, 0, 0, 1}, {0, 1, -1, 0}};
    uint8_t state = encoderState();

    if (state == _nextEncoderState && state != _prevEncoderState) {
      if (++_nextEncoderStateCount >= 2) {
        if (_nextEncoderState != _prevEncoderState) {
          int8_t c = transition[_prevEncoderState][_nextEncoderState];
          if (((c < 0) && (_encoderDirCount > -127)) || ((c>0) && (_encoderDirCount < 127))) {
            _encoderDirCount += c;
          }
          _prevEncoderState = _nextEncoderState;
          _nextEncoderStateCount = 0;
        }
      } 
    } else {
      _nextEncoderState = state;
      _nextEncoderStateCount = 0;
    }
  }

  bool isUp() {
    bool ret = false;
    if (_encoderDirCount > 3) {
      _encoderDirCount -= 4;
      ret = true;
    }
    return ret;
  }  
  
  bool isDown() {
    bool ret = false;
    if (_encoderDirCount < -3) {
      _encoderDirCount += 4;
      ret = true;
    }
    return ret;
  }  

  private:
  uint8_t encoderState() const {
    return  (digitalRead(_a) ? 0 : 2) | (digitalRead(_b) ? 0 : 1);
  }

  int8_t _encoderDirCount;
  uint8_t _prevEncoderState;
  uint8_t _nextEncoderState;
  uint8_t _nextEncoderStateCount;
  uint8_t _a, _b;
};

#endif