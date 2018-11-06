class ActionAttachMetalWire: ActionSingleUseBase
{
	void ActionAttachMetalWire()
	{
		m_MessageSuccess = "Metal wire attached.";
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNonRuined(UAMaxDistances.DEFAULT);
	}

	override int GetType()
	{
		return AT_ATTACH_METAL_WIRE;
	}
		
	override string GetText()
	{
		return "#attach_metal_wire";
	}
	
	override ActionData CreateActionData()
	{
		AttachActionData action_data = new AttachActionData;
		return action_data;
	}
	
	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extra_data = NULL)
	{
		ref InventoryLocation il = new InventoryLocation;
		if (!GetGame().IsMultiplayer() || GetGame().IsClient())
		{
			EntityAI target_entity = EntityAI.Cast( target.GetObject() );
			if(!target_entity.GetInventory().FindFirstFreeLocationForNewEntity( item.GetType(), FindInventoryLocationType.ATTACHMENT, il ))
				return false;
		}
			
		if ( super.SetupAction( player, target, item, action_data, extra_data))
		{
			if (!GetGame().IsMultiplayer() || GetGame().IsClient())
			{
				AttachActionData action_data_a = AttachActionData.Cast(action_data);
				action_data_a.m_AttSlot = il.GetSlot();
			}
			return true;
		}
		return false;
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{
		EntityAI target_EAI = EntityAI.Cast( target.GetObject() );
		
		if ( target_EAI.GetInventory().CanAddAttachment(item) )
		{
			return true;
		}
		
		return false;
	}
	
/*	override void WriteToContext (ParamsWriteContext ctx, ActionData action_data)
	{
		super.WriteToContext(ctx, action_data);
		AttachActionData action_data_a = AttachActionData.Cast(action_data);
		ctx.Write(action_data_a.m_AttSlot);
	}
	
	override bool ReadFromContext(ParamsReadContext ctx, out ActionReciveData action_recive_data )
	{
		if(!action_recive_data)
		{
			action_recive_data = new AttachActionReciveData;
		}
		if (super.ReadFromContext(ctx, action_recive_data ))
		{
			AttachActionReciveData recive_data_a = AttachActionReciveData.Cast(action_recive_data);
			if (ctx.Read(recive_data_a.m_AttSlot))
				return true;
		} 
		return false;
	}
	
	override void HandleReciveData(ActionReciveData action_recive_data, ActionData action_data)
	{
		super.HandleReciveData(action_recive_data, action_data);
		
		AttachActionReciveData recive_data_a = AttachActionReciveData.Cast(action_recive_data);
		AttachActionData action_data_a = AttachActionData.Cast(action_data);
		
		action_data_a.m_AttSlot = recive_data_a.m_AttSlot;
	}
*/
	override void OnExecuteServer( ActionData action_data )
	{
		if (GetGame().IsMultiplayer())
			return;
		
		AttachActionData action_data_a = AttachActionData.Cast(action_data);
		EntityAI target_EAI = EntityAI.Cast( action_data_a.m_Target.GetObject() ); // cast to ItemBase
		if (target_EAI && action_data_a.m_MainItem)
		{
			action_data_a.m_Player.PredictiveTakeEntityToTargetAttachmentEx(target_EAI, action_data_a.m_MainItem,action_data_a.m_AttSlot);
		}
	}
	override void OnExecuteClient( ActionData action_data )
	{
		AttachActionData action_data_a = AttachActionData.Cast(action_data);
		EntityAI target_EAI = EntityAI.Cast( action_data_a.m_Target.GetObject() ); // cast to ItemBase
		if (target_EAI && action_data_a.m_MainItem)
		{
			action_data_a.m_Player.PredictiveTakeEntityToTargetAttachmentEx(target_EAI, action_data_a.m_MainItem, action_data_a.m_AttSlot);
		}
	}
};