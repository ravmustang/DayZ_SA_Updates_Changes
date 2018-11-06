class ActionPlaceObjectCB : ActiondeployObjectCB
{
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousTime(UATimeSpent.DEFAULT_PLACE);
	}
};

class ActionPlaceObject: ActionDeployObject
{		
	void ActionPlaceObject()
	{
		m_CallbackClass	= ActionPlaceObjectCB;
	}

	override int GetType()
	{
		return AT_PLACE_OBJECT;
	}

	override bool HasProgress()
	{
		return false;
	}
	
	override string GetText()
	{
		return "#place_object";
	}
};
