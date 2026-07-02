#include <viewmodel/ManyPropViewModel.h>

#include <NsCore/ReflectionImplement.h>
#include <NsGui/DependencyData.h>

#include <model/ManyPropViewModel.h>
#include <mvvm/Property.h>

namespace viewmodel {

mvvm::WeakPtr<ManyPropViewModel::ModelType_> ManyPropViewModel::GetModel() const {
    return mvvm::ViewModel::GetModel().StaticCast<ModelType_>();
}

// ------ Integer ------
ManyPropViewModel::IntegerType ManyPropViewModel::GetInteger() const {
    return Noesis::DependencyObject::GetValue<IntegerType>(INTEGER_PROP);
}
void ManyPropViewModel::SetInteger(IntegerType value) {
    Noesis::DependencyObject::SetReadOnlyProperty<IntegerType>(INTEGER_PROP, value);
}

// ------ String ------
const ManyPropViewModel::StringType & ManyPropViewModel::GetString() const {
    return Noesis::DependencyObject::GetValue<StringType>(STRING_PROP);
}
void ManyPropViewModel::SetString(const char * pValue) {
    Noesis::DependencyObject::SetReadOnlyProperty<StringType>(STRING_PROP, pValue);
}

// ------ Float ------
ManyPropViewModel::FloatType ManyPropViewModel::GetFloat() const {
    return Noesis::DependencyObject::GetValue<FloatType>(FLOAT_PROP);
}
void ManyPropViewModel::SetFloat(FloatType value) {
    Noesis::DependencyObject::SetReadOnlyProperty<FloatType>(FLOAT_PROP, value);
}

// ------ Boolean ------
ManyPropViewModel::BooleanType ManyPropViewModel::GetBoolean() const {
    return Noesis::DependencyObject::GetValue<BooleanType>(BOOLEAN_PROP);
}
void ManyPropViewModel::SetBoolean(BooleanType value) {
    Noesis::DependencyObject::SetReadOnlyProperty<BooleanType>(BOOLEAN_PROP, value);
}

void ManyPropViewModel::_Observe(const mvvm::SharedPtr<mvvm::Model> & pModel) {
    ParentType_::_Observe(pModel);

    const auto pDerivedModel = GetModel();

    SetInteger(pDerivedModel->GetInteger());
    SetString(pDerivedModel->GetString().c_str());
    SetFloat(pDerivedModel->GetFloat());
    SetBoolean(pDerivedModel->GetBoolean());
}

mvvm::ViewModel::HandlerResult ManyPropViewModel::_HandleModelChanged(
    const mvvm::BaseProperty & property,
    const void * pOldValue,
    const void * pNewValue
) {
    if (ParentType_::_HandleModelChanged(property, pOldValue, pNewValue) == HandlerResult::Handled)
        return HandlerResult::Handled;

    if (property == ModelType_::INTEGER_PROP)
        SetInteger(ModelType_::INTEGER_PROP.Cast(pNewValue));
    else if (property == ModelType_::STRING_PROP)
        SetString(ModelType_::STRING_PROP.Cast(pNewValue).c_str());
    else if (property == ModelType_::FLOAT_PROP)
        SetFloat(ModelType_::FLOAT_PROP.Cast(pNewValue));
    else if (property == ModelType_::BOOLEAN_PROP)
        SetBoolean(ModelType_::BOOLEAN_PROP.Cast(pNewValue));
    else
        return HandlerResult::NotHandled;

    return HandlerResult::Handled;
}

NS_IMPLEMENT_REFLECTION(ManyPropViewModel, "viewmodel.ManyPropViewModel") {
    Noesis::DependencyData * data = NsMeta<Noesis::DependencyData>(Noesis::TypeOf<SelfClass>());

    data->RegisterPropertyRO<IntegerType>(
        INTEGER_PROP,
        "Integer",
        Noesis::PropertyMetadata::Create(IntegerType {})
    );
    data->RegisterPropertyRO<StringType>(
        STRING_PROP,
        "String",
        Noesis::PropertyMetadata::Create(StringType {})
    );
    data->RegisterPropertyRO<FloatType>(
        FLOAT_PROP,
        "Float",
        Noesis::PropertyMetadata::Create(FloatType {})
    );
    data->RegisterPropertyRO<BooleanType>(
        BOOLEAN_PROP,
        "Boolean",
        Noesis::PropertyMetadata::Create(BooleanType {})
    );
}

}
