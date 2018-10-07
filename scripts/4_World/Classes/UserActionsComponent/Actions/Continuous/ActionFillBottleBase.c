class ActionFillBottleBaseCB : ActionContinuousBaseCB
{
	private const float QUANTITY_FILLED_PER_SEC = 50;
	private int m_liquid_type;
	
	override void CreateActionComponent()
	{
		m_liquid_type = ActionFillBottleBase.Cast( m_ActionData.m_Action ).GetLiquidType( m_ActionData.m_Player, m_ActionData.m_Target, m_ActionData.m_MainItem );
		
		m_ActionData.m_ActionComponent = new CAContinuousFill(QUANTITY_FILLED_PER_SEC, m_liquid_type);
	}
};

class ActionFillBottleBase: ActionContinuousBase
{
	private const float WATER_DEPTH = 0.5;
	
	void ActionFillBottleBase()
	{
		m_CallbackClass = ActionFillBottleBaseCB;
		m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FILLBOTTLEPOND;
		m_FullBody = true;
		m_StanceMask = 0;
		
		m_MessageStartFail = "It's ruined.";
		m_MessageStart = "I have started filling the bottle.";
		m_MessageSuccess = "I have finished filling the bottle..";
		m_MessageFail = "Player moved and filling the bottle was canceled.";
		m_MessageCancel = "I stopped filling the bottle.";
		m_SpecialtyWeight = UASoftSkillsWeight.PRECISE_LOW;
	}
	
	override void CreateConditionComponents()  
	{
		m_ConditionItem = new CCINonRuined;
		m_ConditionTarget = new CCTNone;
	}

	override int GetType()
	{
		return AT_FILL_BOTTLE;
	}

	override bool Consumable()
	{
		return true;
	}

	override string GetText()
	{
		PlayerBase player = PlayerBase.Cast( GetGame().GetPlayer() );
		ItemBase item = player.GetItemInHands();

		return "Fill "+item.GetDisplayName().Substring(0,(item.GetDisplayName().Length() )); //crops the '' bit from the displayname
		//return " fill bottle"; //default
	}

	override bool ActionCondition( PlayerBase player, ActionTarget target, ItemBase item )
	{	
		if ( GetGame().IsServer() && GetGame().IsMultiplayer() )
			return true;
		if ( GetLiquidType( player,target,item ) != -1 && !player.IsPlacingLocal() )
			return true;
		/*if ( item.GetQuantity() < item.GetQuantityMax() )
			return true;
		*/
		return false;
	}
	
	override bool ActionConditionContinue( ActionData action_data )
	{
		if (action_data.m_MainItem.GetQuantity() < action_data.m_MainItem.GetQuantityMax())
		{
			return true;
		}
		return false;
	}
	
	override bool SetupAction(PlayerBase player, ActionTarget target, ItemBase item, out ActionData action_data, Param extraData = NULL)
	{	
		SetupStance( player );
	
		if( super.SetupAction(player, target, item, action_data, extraData ))
		{
			if ( target.GetObject() )
			{
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FILLBOTTLEWELL;
			}
			else
			{
				m_CommandUID = DayZPlayerConstants.CMD_ACTIONFB_FILLBOTTLEPOND;
			}
			
			return true;
		}
		return false;
	}
	
	override void WriteToContext(ParamsWriteContext ctx, ActionData action_data)
	{
		super.WriteToContext(ctx, action_data);
		
		if( HasTarget() )
		{
			ctx.Write(action_data.m_Target.GetCursorHitPos()); //sends cursor pos for pond recognition
		}
	}
	
	override bool ReadFromContext(ParamsReadContext ctx, ActionData action_data )
	{		
		super.ReadFromContext(ctx, action_data);
		
		if( HasTarget() )
		{
			vector cursor_position;
			if ( !ctx.Read(cursor_position) )
				return false;
			action_data.m_Target.SetCursorHitPos(cursor_position);
		}
		return true;
	}

	
	//TODO
	/*protected void CreateAndSetupActionCallback( ActionData action_data )
	{
		//Print("ActionBase.c | CreateAndSetupActionCallback | DBG ACTION CALLBACK CREATION CALLED");
		ActionBaseCB callback;
		if (  IsFullBody(action_data.m_Player) )
		{
			Class.CastTo(callback, action_data.m_Player.StartCommand_Action(GetActionCommand(action_data.m_Player),GetCallbackClassTypename(),GetStanceMask(action_data.m_Player)));	
			//Print("ActionBase.c | CreateAndSetupActionCallback |  DBG command starter");		
		}
		else
		{
			Class.CastTo(callback, action_data.m_Player.AddCommandModifier_Action(GetActionCommand(action_data.m_Player),GetCallbackClassTypename()));
			//Print("ActionBase.c | CreateAndSetupActionCallback |  DBG command modif starter: "+callback.ToString()+"   id:"+GetActionCommand().ToString());
			
		}
		callback.SetActionData(action_data); 
		callback.InitActionComponent(); //jtomasik - tohle mozna patri do constructoru callbacku?
		action_data.m_Callback = callback;
	}*/
	
	int GetLiquidType( PlayerBase player, ActionTarget target, ItemBase item )
	{
		vector pos_cursor = target.GetCursorHitPos();
		if( g_Game.SurfaceIsPond(pos_cursor[0], pos_cursor[2]) || (target.GetObject() && (target.GetObject().GetType() == "Land_Misc_Well_Pump_Yellow" || target.GetObject().GetType() == "Land_Misc_Well_Pump_Blue")) )
		{
			if ( vector.Distance(player.GetPosition(), pos_cursor) < UAMaxDistances.DEFAULT && Liquid.CanFillContainer(item, LIQUID_WATER ) )
			{
				return LIQUID_WATER;
			}
		}
		else if(target.GetObject() && target.GetObject().GetType() == "Land_FuelStation_Feed")
		{
			if ( vector.Distance(player.GetPosition(), pos_cursor) < UAMaxDistances.DEFAULT && Liquid.CanFillContainer(item, LIQUID_GASOLINE ) )
			{
				return LIQUID_GASOLINE;
			}
		}
		return -1;
	}
	
	void SetupStance( PlayerBase player )
	{
		//returns in format (totalWaterDepth, characterDepht, 0)
		vector water_info = HumanCommandSwim.WaterLevelCheck( player, player.GetPosition() );
		if ( water_info[1] > WATER_DEPTH )
		{
			m_StanceMask = DayZPlayerConstants.STANCEMASK_ERECT;
		}
		else
		{
			m_StanceMask = DayZPlayerConstants.STANCEMASK_CROUCH;
		}
	}
};