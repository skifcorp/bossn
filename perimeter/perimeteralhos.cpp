#include "perimeteralhos.h"
#include "settingstool.h"


BossnFactoryRegistrator<PerimeterControlByWeight> PerimeterControlByWeight::registator("PerimeterControlByWeight");


void PerimeterControlByWeight::setSettings( const QMap<QString, QVariant>& s)
{
    weight_name = get_setting<QString>("weightName", s);
    min_weight  = get_setting<float>  ("minWeight" , s);
    get_weight_method      = get_setting<QString>("method"    , s);
}

bool PerimeterControlByWeight::appeared(AlhoSequence * caller)
{
    bool ret = false;

    QVariant val = tags_[weight_name]->func(get_weight_method, caller);

    if ( !val.isValid() ) return false;

    if ( val.toInt() > min_weight && !was_appeared) {
        was_appeared = true;
        ret = true;
    }

    return ret;
}

bool PerimeterControlByWeight::disappeared(AlhoSequence * caller)
{
    bool ret = false;

    QVariant val = tags_[weight_name]->func(get_weight_method, caller);

    if (!val.isValid()) return false;

    if ( val.toInt() < min_weight && was_appeared) {
        was_appeared = false;
        ret = true;
    }

    return ret;
}

#include "systemtrayiconreceiver_p.h"

void SystemTrayIconEventsReceiver::onOn()
{
    is_on = true;
    emit onPressed();
}

void SystemTrayIconEventsReceiver::onOff()
{
    is_on = false;
    emit offPressed();
}

void SystemTrayIconEventsReceiverDeleter::operator ()(SystemTrayIconEventsReceiver * s)
{
    delete s;
}


BossnFactoryRegistrator<PerimeterControlManualEmulator> PerimeterControlManualEmulator::registator("PerimeterControlManualEmulator");

PerimeterControlManualEmulator::PerimeterControlManualEmulator(Tags & t) : PerimeterControl(t),
    tray(new SystemTrayIconEventsReceiver, SystemTrayIconEventsReceiverDeleter()), was_on_weights(false)
{

}



void PerimeterControlManualEmulator::setSettings( const QMap<QString, QVariant>& )
{

}

bool PerimeterControlManualEmulator::appeared(AlhoSequence * )
{
    if (!was_on_weights && tray->isOn()) {
        was_on_weights = true;
        return true;
    }
    return false;
}

bool PerimeterControlManualEmulator::disappeared(AlhoSequence * )
{
    if ( !tray->isOn() && was_on_weights ) {
        was_on_weights = false;
        return true;
    }

    return false;
}


BossnFactoryRegistrator<PerimeterControlCyclicEmulator> PerimeterControlCyclicEmulator::registator("PerimeterControlCyclicEmulator");

PerimeterControlCyclicEmulator::PerimeterControlCyclicEmulator(Tags & t) : PerimeterControl(t),
    tray(new SystemTrayIconEventsReceiver, SystemTrayIconEventsReceiverDeleter()), was_on_weights(false)
{
    connect(&timer_, SIGNAL(timeout()), this, SLOT(onTimer()));
    connect( tray.get(), SIGNAL(onPressed()), this, SLOT(onOnPressed()) );
    connect( tray.get(), SIGNAL(offPressed()), this, SLOT(onOffPressed()) );
}



void PerimeterControlCyclicEmulator::setSettings( const QMap<QString, QVariant>& s)
{
    timer_.setInterval( get_setting<int>("period", s) );
}

bool PerimeterControlCyclicEmulator::appeared(AlhoSequence * )
{
    if ( !was_on_weights && timer_processed ) {
        was_on_weights = true;
        return true;
    }
    return false;
}

bool PerimeterControlCyclicEmulator::disappeared(AlhoSequence * )
{
    if ( was_on_weights && timer_processed ) {
        was_on_weights = false;
        return true;
    }

    return false;
}

void PerimeterControlCyclicEmulator::onTimer()
{
    timer_processed = true;
}

void PerimeterControlCyclicEmulator::onOnPressed()
{
    timer_.start();
}

void PerimeterControlCyclicEmulator::onOffPressed()
{
    timer_.stop();
}
