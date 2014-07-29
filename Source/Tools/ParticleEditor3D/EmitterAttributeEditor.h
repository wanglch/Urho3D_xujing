#pragma once
#include <QComboBox>
#include <QLineEdit>
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
	void HandleEmitterTypeEditorChanged(int value);
	void HandleSelectMaterialClick();
	void HandleMaxParticlesEditorValueChanged(int value);
	void HandleUpdateInvisibleChanged(int value);

	void HandleRelativeChanged(int value);
	void HandleActiveTimeChanged(float value);
	void HandleInActiveTimeChanged(float value);
	void HandleEmissionRateChanged(float value);
	void HandleIsSortedChanged(int value);
private:
	virtual void HandleUpdateWidget();

	void CreateEmitterTypeEditor();
	void CreateSelectMaterialEditor();
	void CreateMaxParticleEditor();
	//�����Ӳ��ɼ�ʱ�Ƿ����
	void CreateUpdateInvisible();

	void CreateRelative();
	void CreateActiveTime();
	void CreateInActiveTime();
	void CreateEmissionRate();
	void CreateIsSorted();

	void HandlePostUpdate(StringHash eventType, VariantMap& eventData);

	QComboBox* emitterTypeEditor_;
	QLineEdit* materialPathEditor_;
	IntEditor* maxParticleEditor_;
	FloatEditor* activeTimeEditor_;
	FloatEditor* inActiveTimeEditor_;
	FloatEditor* emissionRateMinEditor_;
	FloatEditor* emissionRateMaxEditor_;
};
