/*
 * Motherboard
 * Ghost In Translation
 * version: 2.0.0-beta.3
 */

#ifndef Motherboard_h
#define Motherboard_h

#include "SystemExclusiveManager.h"
#include "Input.h"
#include "InputButton.h"
#include "InputGate.h"
#include "InputRotary.h"
#include "InputTrigger.h"
#include "MidiInput.h"
#include "Output.h"
#include "OutputGate.h"
#include "OutputLed.h"
#include "OutputTrigger.h"
#include "Vca.h"
#include "Vcc.h"

class Motherboard
{

private:
    // Singleton
    static Motherboard *instance;
    Motherboard();

public:
    static Motherboard *getInstance();
};

// Singleton pre init
Motherboard *Motherboard::instance = nullptr;

inline Motherboard::Motherboard()
{
    SystemExclusiveManager::getInstance();
}

/**
   Singleton instance
*/
inline Motherboard *Motherboard::getInstance()
{
    if (!instance)
        instance = new Motherboard;
    return instance;
}

Motherboard *motherboard = Motherboard::getInstance();
#endif