/****************************************************************************
**
** Copyright (C) 2012 Digia Plc and/or its subsidiary(-ies).
** Contact: http://www.qt-project.org/legal
**
** This file is part of the Qt Compositor.
**
** $QT_BEGIN_LICENSE:BSD$
** You may use this file under the terms of the BSD license as follows:
**
** "Redistribution and use in source and binary forms, with or without
** modification, are permitted provided that the following conditions are
** met:
**   * Redistributions of source code must retain the above copyright
**     notice, this list of conditions and the following disclaimer.
**   * Redistributions in binary form must reproduce the above copyright
**     notice, this list of conditions and the following disclaimer in
**     the documentation and/or other materials provided with the
**     distribution.
**   * Neither the name of Digia Plc and its Subsidiary(-ies) nor the names
**     of its contributors may be used to endorse or promote products derived
**     from this software without specific prior written permission.
**
**
** THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
** "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
** LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
** A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
** OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
** SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
** LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
** DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
** THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
** (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
** OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE."
**
** $QT_END_LICENSE$
**
****************************************************************************/

#ifndef WAYLAND_OBJECT_H
#define WAYLAND_OBJECT_H

#include <wayland-server.h>
#include <string.h>

#include <QtCompositor/qwaylandexport.h>

QT_BEGIN_NAMESPACE

namespace QtWayland {

template <typename T>
class Object : public T
{
public:
    typedef T Base;

    Object() { memset(this, 0, sizeof(T)); }

    const T *base() const { return this; }
    T *base() { return this; }

    template <typename Implementation>
    void addClientResource(wl_client *client,
                           wl_resource *resource,
                           int id, const struct wl_interface *interface,
                           Implementation implementation,
                           void (*destroy)(struct wl_resource *resource))
    {
        resource->object.id = id;
        resource->object.interface = interface;
        resource->object.implementation = (void (**)(void))implementation;
        resource->data = this;
        resource->destroy = destroy;

        wl_client_add_resource(client, resource);
    }
};

template <typename T>
T *resolve(wl_resource *from)
{
    Object<typename T::Base> *object = reinterpret_cast<Object<typename T::Base> *>(from->data);
    return static_cast<T *>(object);
}

template <typename T>
T *wayland_cast(typename T::Base *from)
{
    Object<typename T::Base> *object = reinterpret_cast<Object<typename T::Base> *>(from);
    return static_cast<T *>(object);
}

}

QT_END_NAMESPACE

#endif //WAYLAND_OBJECT_H
