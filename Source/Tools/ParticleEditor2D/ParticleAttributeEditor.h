//
// Copyright (c) 2014 the ParticleEditor2D project.
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
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.
//

#pragma once

#include "ParticleEffectEditor.h"
#include "Widgets/ScrollAreaWidget.h"

namespace Urho3D
{

class ColorVarianceEditor;
class ValueVarianceEditor;

class ParticleAttributeEditor : public ScrollAreaWidget, public ParticleEffectEditor
{
    Q_OBJECT
        OBJECT(ParticleAttributeEditor)
public:
    ParticleAttributeEditor(Context* context);
    virtual ~ParticleAttributeEditor();

private slots:
    void HanldeValueVarianceEditorValueChanged(float average, float variance);
    void HandleStartColorEditorValueChanged(const Color& average, const Color& variance);
    void HandleFinishColorEditorValueChanged(const Color& average, const Color& variance);

private:
    /// Handle update widget.
    virtual void HandleUpdateWidget();
    /// Create value variance editor.
    ValueVarianceEditor* CreateValueVarianceEditor(const QString& name, float min, float max);

    /// Particle life span editor.
    ValueVarianceEditor* particleLifeSpanEditor_;
    /// Start size editor.
    ValueVarianceEditor* startSizeEditor_;
    /// Finish size editor.
    ValueVarianceEditor* finishSizeEditor_;
    /// Start rotation editor.
    ValueVarianceEditor* startRotationEditor_;
    /// Finish rotation editor.
    ValueVarianceEditor* finishRotationEditor_;

    /// Start color editor.
    ColorVarianceEditor* startColorEditor_;
    /// Finish color editor.
    ColorVarianceEditor* finishColorEditor_;
};

}
