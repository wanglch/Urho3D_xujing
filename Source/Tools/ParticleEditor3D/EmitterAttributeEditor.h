#pragma once
#include "ParticleEffectEditor.h"

/// ������������Ա༭

class EmitterAttributeEditor : public ScrollAreaWidget, public ParticleEffectEditor
{
	Q_OBJECT
	OBJECT(EmitterAttributeEditor)
public:
	EmitterAttributeEditor(Context* context);
	virtual ~EmitterAttributeEditor();
private slots:

private:
	virtual void HandleUpdateWidget();

};
