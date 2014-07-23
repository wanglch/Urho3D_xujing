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

#include <QHBoxLayout>

class QLabel;
class QSpinBox;
class QSlider;

namespace Urho3D
{

class IntEditor : public QHBoxLayout
{
    Q_OBJECT

public:
    IntEditor(const QString& text);
    virtual ~IntEditor();

public:
    /// Set value.
    void setValue(int value);
    /// Return value.
    int value() const;

    /// Set range.
    void setRange(int min, int max);
    /// Return range.
    void GetRange(int& min, int& max) const;

signals:
    void valueChanged(int);

protected slots:
    void sliderValueChanged(int value);
    void spinBoxValueChanged(int value);

private:
    QLabel* label_ ;
    QSlider* slider_;
    QSpinBox* spinBox_;
    bool senderIsSlider_;
    bool senderIsSpinBox_;
};

}