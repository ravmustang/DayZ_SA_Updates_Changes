class CAContinuousMineWood : CAContinuousBase
{
	protected float 				m_TimeElpased;
	protected float 				m_AdjustedTimeBetweenMaterialDrops;
	protected float 				m_TimeBetweenMaterialDrops;
	protected float					m_DamageToMiningItemEachDrop;
	protected float					m_AdjustedDamageToMiningItemEachDrop;
	protected int 					m_AmountOfDrops;
	//protected string				m_Material;
	//protected float 				m_AmountOfMaterialPerDrop;
	protected ref map<string,int> 	m_MaterialAndQuantityMap;
	protected float					m_TimeToComplete;
	protected ref Param1<float>		m_SpentUnits;
	protected bool					m_DataLoaded = false;
	protected ItemBase				m_MinedItem[5];
	
	void CAContinuousMineWood(float time_between_drops)
	{
		m_TimeBetweenMaterialDrops = time_between_drops;
	}
	
	override void Setup( ActionData action_data )
	{
		m_TimeElpased = 0;
		if ( !m_SpentUnits )
		{ 
			m_SpentUnits = new Param1<float>(0);
		}
		else
		{	
			m_SpentUnits.param1 = 0;
		}
		m_MaterialAndQuantityMap = new map<string,int>;
		m_DataLoaded = GetMiningData(action_data);
		for(int i = 0; i < m_MaterialAndQuantityMap.Count(); i++)
		{
			Print("material = " + m_MaterialAndQuantityMap.GetKey(i) + "; quantity = " + m_MaterialAndQuantityMap.GetElement(i));
		}

		m_AdjustedTimeBetweenMaterialDrops = action_data.m_Player.GetSoftSkillManager().SubtractSpecialtyBonus( m_TimeBetweenMaterialDrops, m_Action.GetSpecialtyWeight(), true );		
		m_TimeToComplete = m_AmountOfDrops * m_AdjustedTimeBetweenMaterialDrops;
	}
	
	override int Execute( ActionData action_data )
	{
		Object targetObject;
		Class.CastTo(targetObject, action_data.m_Target.GetObject());
		if ( !action_data.m_Player || !m_DataLoaded )
		{
			return UA_ERROR;
		}
		
		if ( action_data.m_MainItem.IsDamageDestroyed() || targetObject.IsDamageDestroyed() )
		{
			return UA_FINISHED;
		}
		else
		{
			if ( m_TimeElpased < m_AdjustedTimeBetweenMaterialDrops )
			{
				m_TimeElpased += action_data.m_Player.GetDeltaT();
			}
			else
			{
				if ( GetGame().IsServer() )
				{
					CreateItems(action_data);
					
					float damage = (1 / m_AmountOfDrops) * 100;
					targetObject.DecreaseHealth("","",damage,true);					
					action_data.m_MainItem.DecreaseHealth ( "", "", m_AdjustedDamageToMiningItemEachDrop );
				}
				if ( targetObject.IsDamageDestroyed() )
				{
					if ( m_SpentUnits )
					{
						m_SpentUnits.param1 = m_TimeElpased;
						SetACData(m_SpentUnits);	
					}
					if ( targetObject.IsTree() )
						ItemBase wooden_logs = ItemBase.Cast(GetGame().CreateObject("WoodenLog",action_data.m_Player.GetPosition(), false));
					
					targetObject.OnTreeCutDown( action_data.m_MainItem );
					OnCompletePogress(action_data);
					return UA_FINISHED;
				}
				m_TimeElpased = m_TimeElpased - m_AdjustedTimeBetweenMaterialDrops;
				OnCompletePogress(action_data);
			}
			return UA_PROCESSING;
		}
	}
	
	override float GetProgress()
	{	
		//float progress = m_TimeElpased/m_AdjustedTimeBetweenMaterialDrops;
		return m_TimeElpased/m_AdjustedTimeBetweenMaterialDrops;
	}
		
	override int Cancel( ActionData action_data )
	{	
		ToolBase cut_tree_tool = ToolBase.Cast( action_data.m_MainItem );
		
		if (!cut_tree_tool)
			return super.Cancel( action_data );
		
		if ( cut_tree_tool.GetHaveCutHardTree() || cut_tree_tool.GetHaveCutSoftTree() || cut_tree_tool.GetHaveCutHardBush() || cut_tree_tool.GetHaveCutSoftBush() )
		{
			cut_tree_tool.SoundSynchRemoteReset();
		}
		
		return super.Cancel( action_data );
	}
	
	//---------------------------------------------------------------------------
	
	bool GetMiningData(ActionData action_data )
	{
		WoodBase ntarget;
		if ( Class.CastTo(ntarget, action_data.m_Target.GetObject()) )
		{
			m_AmountOfDrops = Math.Max(1,ntarget.GetAmountOfDrops(action_data.m_MainItem));
			//m_Material = ntarget.GetMaterial(action_data.m_MainItem);
			//m_AmountOfMaterialPerDrop = Math.Max(1,ntarget.GetAmountOfMaterialPerDrop(action_data.m_MainItem));
			ntarget.GetMaterialAndQuantityMap(action_data.m_MainItem,m_MaterialAndQuantityMap);
			m_DamageToMiningItemEachDrop = ntarget.GetDamageToMiningItemEachDrop(action_data.m_MainItem);
			m_AdjustedDamageToMiningItemEachDrop = action_data.m_Player.GetSoftSkillManager().SubtractSpecialtyBonus( m_DamageToMiningItemEachDrop, m_Action.GetSpecialtyWeight(), true );
			return true;
		}
		return false;
	}
	
	void CreateItems(ActionData action_data)
	{
		string material;
		int increment;
		float adjusted_increment;
		for(int i = 0; i < m_MaterialAndQuantityMap.Count(); i++)
		{
			material = m_MaterialAndQuantityMap.GetKey(i);
			if (material != "")
			{
				increment = m_MaterialAndQuantityMap.GetElement(i);
				adjusted_increment = action_data.m_Player.GetSoftSkillManager().AddSpecialtyBonus( increment, m_Action.GetSpecialtyWeight(), true, 0 );
				adjusted_increment = Math.Round( adjusted_increment );
				if ( !m_MinedItem[i] )
				{
					m_MinedItem[i] = ItemBase.Cast(GetGame().CreateObject(material,action_data.m_Player.GetPosition(), false));
					m_MinedItem[i].SetQuantity(adjusted_increment);
				}
				else 
				{
					if ( (m_MinedItem[i].GetQuantity() + increment) >= m_MinedItem[i].GetQuantityMax() )
					{
						increment -= m_MinedItem[i].GetQuantityMax() - m_MinedItem[i].GetQuantity();
						adjusted_increment = action_data.m_Player.GetSoftSkillManager().AddSpecialtyBonus( increment, m_Action.GetSpecialtyWeight(), true, 0 );
						adjusted_increment = Math.Round( adjusted_increment );
						m_MinedItem[i].SetQuantityMax();
						m_MinedItem[i] = ItemBase.Cast(GetGame().CreateObject(material,action_data.m_Player.GetPosition(), false));
						m_MinedItem[i].SetQuantity(adjusted_increment,false);
					}
					else
					{
						m_MinedItem[i].AddQuantity(adjusted_increment,false);
					}
				}
			}
		}
	}
	
};