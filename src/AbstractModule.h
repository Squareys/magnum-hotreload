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

#define MODULE_PLUGIN_ID "de.squareys.HotReloadExample.AbstractModule/1.0"

/**
@brief Hot reloadable module
*/
class AbstractModule: public Corrade::PluginManager::AbstractPlugin {
    CORRADE_PLUGIN_INTERFACE(MODULE_PLUGIN_ID)

    public:
        AbstractModule(Corrade::PluginManager::AbstractManager& manager, const std::string& plugin): AbstractPlugin{manager, plugin} {}

        /**
         * @brief Method called to load this module.
         * @param previousState Potential previous state of this plugin, if hot reloaded, may be `nullptr`.
         */
        virtual void load(void* previousState=nullptr) = 0;

        /**
         * @brief Method called when this module is unloaded.
         * @param shutdown `true` if the application is shutting down and no
         *                 state should be retained. `false` in case of hot
         *                 reload.
         * @returns State of the plugin to pass onto a hot reloaded version of
         *          itself or `nullptr`, if no state or state retention is not
         *          supported by this module.
         */
        virtual void* unload(bool shutdown=true) = 0;
};

#endif
