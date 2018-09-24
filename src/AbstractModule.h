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

#ifndef src_AbstractModule_h_
#define src_AbstractModule_h_

#include <Corrade/PluginManager/Manager.h>
#include <Corrade/PluginManager/AbstractPlugin.h>
#include <Corrade/Containers/Array.h>

#define MODULE_PLUGIN_ID "de.squareys.HotReloadExample.AbstractModule/1.0"

/**
@brief Hot reloadable module
*/
class AbstractModule: public Corrade::PluginManager::AbstractPlugin {
    CORRADE_PLUGIN_INTERFACE(MODULE_PLUGIN_ID)

    public:
        AbstractModule(Corrade::PluginManager::AbstractManager& manager, const std::string& plugin):
            AbstractPlugin{manager, plugin} {}

        /**
         * @brief Method called to load this module.
         * @param previousState Potential previous state of this plugin, if hot
         *                      reloaded, may be empty if first load or
         *                      implementation of `unload()` returned an empty
         *                      array.
         *
         * Use `previousState` with release to prevent data from being destroyed:
         *
         * @code
         *   _myState = static_cast<MyState*>(previousState.release());
         * @endcode
         */
        virtual void load(Corrade::Containers::Array<void>&& previousState) = 0;
        virtual void load() { load(std::move(Corrade::Containers::Array<void>{nullptr})); }

        /**
         * @brief Method called when this module is unloaded.
         * @param shutdown `true` if the application is shutting down and no
         *                 state should be retained. `false` in case of hot
         *                 reload.
         * @returns State of the plugin to pass onto a hot reloaded version of
         *          itself or an empty array, if no state exists or state
         *          retention is not supported by this module.
         *
         * Warning: While it is possible to pass on a deleter with the Array,
         * this deleter will get unloaded with the module, if it isn't kept
         * around. Whether that works or not will therefore depend on how you
         * handle unloading.
         */
        virtual Corrade::Containers::Array<void>&& unload(bool shutdown=true) = 0;
};

#endif
