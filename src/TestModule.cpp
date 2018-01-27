/*
    Copyright © 2018 Jonathan Hale <squareys@googlemail.com>

    Permission is hereby granted, free of charge, to any person obtaining a
    copy of this software and associated documentation files (the "Software"),
    to deal in the Software without restriction, including without limitation
    the rights to use, copy, modify, merge, publish, distribute, sublicense,
    and/or sell copies of the Software, and to permit persons to whom the
    Software is furnished to do so, subject to the following conditions:

    The above copyright notice and this permission notice shall be included
    in all copies or substantial portions of the Software.

    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
    THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
    DEALINGS IN THE SOFTWARE.
*/

#include "AbstractModule.h"

#include <Corrade/Utility/Debug.h>

using namespace Corrade::Utility;

/* State to be shared between hot reloaded instancees of this plugin */
struct TestModuleState {
    int hotReloadCounter = 0;
};

/**
@brief Super simple implementation of AbstractModule for manual testing purposes.
*/
class TestModule: public AbstractModule {
    public:
        TestModule(Corrade::PluginManager::AbstractManager& manager, const std::string& plugin):
            AbstractModule{manager, plugin} {}

        virtual void load(void* previousState) override {
            Debug{} << "TestModule::load()";

            if(!previousState) {
                _state = new TestModuleState;
            } else {
                _state = static_cast<TestModuleState*>(previousState);
                _state->hotReloadCounter++;

                Debug() << "I have been hot reloaded" << _state->hotReloadCounter << "times.";
            }
        }

        virtual void* unload(bool shutdown) override {
            Debug{} << "TestModule::unload(" << shutdown << ")";
            return _state;
        }

    private:
        TestModuleState* _state;
};

CORRADE_PLUGIN_REGISTER(TestModule, TestModule, MODULE_PLUGIN_ID);
