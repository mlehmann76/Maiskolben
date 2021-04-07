#ifndef __ROTARY_H__
#define __ROTARY_H__

#include <stdint.h>

class Rotary {
  public:

  enum direction { D_NONE, D_UP, D_DOWN } ;

  Rotary(uint8_t a, uint8_t b) : 
    _encoderDir(D_NONE), _encoderDirCount(0), _prevEncoderState(0), 
    _nextEncoderState(0), _nextEncoderStateCount(0), _a(a), _b(b), 
    _clearCount(0) {

    }

  void encoder_poll() {
    static const direction transition[][4] = {
      {D_NONE,D_DOWN,D_UP,D_NONE},
      {D_UP,D_NONE,D_NONE,D_DOWN},
      {D_DOWN, D_NONE, D_NONE, D_UP},
      {D_NONE, D_UP, D_DOWN, D_NONE}};
    uint8_t state = encoderState();
    if (_clearCount) {
      _encoderDirCount = _encoderDirCount > _clearCount ? _encoderDirCount-_clearCount : 0;
      _clearCount = 0;
    }
    if (state == _nextEncoderState && state != _prevEncoderState) {
      if (++_nextEncoderStateCount >= 2) {
        if (_nextEncoderState != _prevEncoderState) {
          _encoderDir = transition[_prevEncoderState][_nextEncoderState];
          if (_encoderDir == D_NONE) {
            _encoderDirCount = 0;
          } else {
            _encoderDirCount++;
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

  uint8_t getUp() const {
    return _encoderDir == D_UP ? _encoderDirCount : 0;
  }  
  
  uint8_t getDown() const {
    return _encoderDir == D_DOWN ? _encoderDirCount : 0;
  }  

  void clear(uint8_t count = 255) {
    _clearCount = count;
  }
  
  private:
  uint8_t encoderState() {
    return  (digitalRead(_a) ? 0 : 2) | (digitalRead(_b) ? 0 : 1);
  }

  direction _encoderDir;
  uint8_t _encoderDirCount;
  uint8_t _prevEncoderState;
  uint8_t _nextEncoderState;
  uint8_t _nextEncoderStateCount;
  uint8_t _a, _b;
  uint8_t _clearCount;
};

#endif