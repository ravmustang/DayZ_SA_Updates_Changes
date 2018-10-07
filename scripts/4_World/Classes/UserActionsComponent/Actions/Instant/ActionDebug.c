class DebugActionData : ActionData
{
	ItemBase 	m_item;
	int			m_DebugActionID;
}

class ActionDebug : ActionInstantBase
{	
	override int GetType()  
	{
		return AT_DEBUG;
	}
	
	override bool HasTarget()
	{
		return false;
	}
	
	override ActionData CreateActionData()
	{
		DebugActionData action_data = new DebugActionData;
		return action_data;
	}
	
	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extraData = NULL)
	{
		DebugActionData dActionData;
		Class.CastTo( dActionData, CreateActionData());
		dActionData.m_Action = this;
		dActionData.m_Player = player;
		
		if (!GetGame().IsMultiplayer() || GetGame().IsClient() )
		{
			ref Param2<ItemBase,int> ndata = Param2<ItemBase,int>.Cast( extraData );
			dActionData.m_MainItem = ndata.param1;
			dActionData.m_DebugActionID = ndata.param2;
		}
		action_data = dActionData;
		
		return true;
	}
	
	override void WriteToContext (ParamsWriteContext ctx, ActionData action_data)
	{
		DebugActionData dActionData;
		Class.CastTo( dActionData, action_data );
		
		ctx.Write(dActionData.m_MainItem);
		ctx.Write(dActionData.m_DebugActionID);
	}
	
	override bool ReadFromContext(ParamsReadContext ctx, ActionData action_data )
	{
		DebugActionData dActionData;
		Class.CastTo( dActionData, action_data );
		
		ItemBase item;
		int debugActionID;
		
		if(!ctx.Read(item))
			return false;
		if(!ctx.Read(debugActionID))
			return false;
		
		dActionData.m_MainItem = item;
		dActionData.m_DebugActionID = debugActionID;
		return true;
	}
	
	override void Start(ActionData action_data)
	{
		DebugActionData dActionData;
		Class.CastTo( dActionData, action_data );
		
		dActionData.m_MainItem.OnAction(dActionData.m_DebugActionID,dActionData.m_Player,NULL);
	}
	
	override bool UseAcknowledgment()
	{
		return false;
	}
	
};