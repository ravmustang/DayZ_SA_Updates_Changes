class ActionEmptyBottleBaseCB : ActionContinuousBaseCB
{
	private const float QUANTITY_EMPTIED_PER_SEC = 200;
	
	override void CreateActionComponent()
	{
		m_ActionData.m_ActionComponent = new CAContinuousEmpty(QUANTITY_EMPTIED_PER_SEC);
	}
};

class ActionEmptyBottleBase: ActionContinuousBase
{
	void ActionEmptyBottleBase()
	{
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONMOD_EMPTYBOTTLE;
		m_MessageStartFail = "It's ruined.";
		m_MessageStart = "I have started filling the bottle.";
		m_MessageSuccess = "I have finished filling the bottle..";
		m_MessageFail = "Player moved and filling the bottle was canceled.";
		m_MessageCancel = "I stopped filling the bottle.";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
		
		m_CallbackClass = ActionEmptyBottleBaseCB;
		m_FullBody = false;
		m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT | DayZPlayerConstants.STANCEMASK_CROUCH;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINotRuinedAndEmpty;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_EMPTY_BOTTLE;
	}

	override bool HasTarget()
	{
		return false;
	}

	override string GetText()
	{
		return "Empty";
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		//TODO: implement proper solution, ideally something to do with camera angle
		vector 	pos_cursor = target.GetCursorHitPos();
		vector 	pos_head;
		MiscGameplayFunctions.GetHeadBonePos( player, pos_head );
		float distance = vector.Distance(pos_cursor, pos_head);
				
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
			return true;
		/*if ( item.IsLiquidPresent() && distance < 1.7 && !target.GetObject() )
			return true;*/
		DayZPlayerCameraBase camera;
		if ( item.IsLiquidPresent() && !target.GetObject() && DayZPlayerCameraBase.CastTo(camera, player.GetCurrentCamera()) && camera.GetCurrentPitch() < -70 )
		{
			//Print(camera.GetCurrentPitch());
			return true;
		}
		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{
		if (action_data.m_MainItem.GetQuantity() > action_data.m_MainItem.GetQuantityMin())
		{
			return true;
		}
		return false;
	}
	
	
	/*override void OnCompleteLoopServer( ActionData action_data )
	{
		//item.TransferModifiers(player);
		Param1<float> nacdata;
		Class.CastTo(nacdata,  action_data.m_ActionComponent.GetACData() );
		float delta = nacdata.param1;
		action_data.m_MainItem.AddQuantity( -delta );

		action_data.m_Player.GetSoftSkillManager().AddSpecialty( m_SpecialtyWeight );
	}
	
	override void OnCancelServer( ActionData action_data )
	{
		OnCompleteLoopServer(action_data);
	}*/
};