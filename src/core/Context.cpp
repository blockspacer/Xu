// MIT License
//
// Copyright (c) 2020 Xu Collaborators
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.

#include <xu/core/Context.hpp>

#include <iostream> // Temporary

namespace xu {

void Context::NotifyEvent(MouseMoveEvent const& evt) {
    switch (inputReception) {
        case InputReception::Queued: {
            Event event;
            event.type = EventType::MouseMove;
            event.data.mouseMove = evt;
            eventQueue.push(event);
            break;
        }
        case InputReception::Immediate: DispatchEvent(evt); break;
    }
}

void Context::NotifyEvent(WindowResizeEvent const& evt) {
    switch (inputReception) {
        case InputReception::Queued: {
            Event event;
            event.type = EventType::WindowResize;
            event.data.windowResize = evt;
            eventQueue.push(event);
            break;
        }
        case InputReception::Immediate: DispatchEvent(evt); break;
    }
}

void Context::ProcessEvents() {
    if (inputReception == InputReception::Queued) {
        while (!eventQueue.empty()) {
            const auto& evt = eventQueue.front();

            switch (evt.type) {
                case EventType::MouseMove:
                    DispatchEvent(evt.data.mouseMove);
                    break;
                case EventType::WindowResize:
                    DispatchEvent(evt.data.windowResize);
                    break;
            }

            eventQueue.pop();
        }
    }

    BuildRenderData();
}

RenderData const& Context::GetRenderData() const { return renderData; }

void Context::DispatchEvent(MouseMoveEvent const& evt) {
    std::cout << "Mouse move: " << evt.position.x << " " << evt.position.y
              << std::endl;
}

void Context::DispatchEvent(WindowResizeEvent const& evt) {
    std::cout << "Window resize: " << evt.size.x << " " << evt.size.y
              << std::endl;
}

void Context::BuildRenderData() {
    // TODO: RenderData::Clear() function
    renderData.cmdLists.clear();
    renderData.vertices.clear();
    renderData.indices.clear();
    CommandList cmdList;
    BuildRenderData(root.get(), cmdList);
    renderData.cmdLists.push_back(cmdList);
}

void Context::BuildRenderData(Widget* widget, CommandList& cmdList) {
    // Add quad to RenderData (temporary, more complex geometry later)
    renderData.PushQuad(cmdList, widget->Geometry());

    for (size_t child = 0; child < widget->NumChildren(); ++child) {
        BuildRenderData(widget->GetChild(child), cmdList);
    }

}

} // namespace xu