class FlashGrenade extends Grenade_Base
{
	override void OnExplosionEffects(Object source, Object directHit, int componentIndex, string surface, vector pos, vector surfNormal, float energyFactor, float explosionFactor, bool isWater, string ammoType)
	{
		super.OnExplosionEffects(source, directHit, componentIndex, surface, pos, surfNormal, energyFactor, explosionFactor, isWater, ammoType);

		PlayerBase player = PlayerBase.Cast(GetGame().GetPlayer());

		if( player )
		{
			bool visual = false;
			vector headPos = player.GetDamageZonePos("Head"); // animated position in the middle of the zone
			
			// get grenade range
			string grenadePath = "cfgAmmo " + ammoType + " indirectHitRange";
			float maxRange = GetGame().ConfigGetFloat(grenadePath);

			if( vector.Distance(headPos, pos) <= maxRange ) 
			{
				// check visibility
				vector contactPos;
				vector contactDir;
				int contactComponent;
				
				// ignore collisions with parent if fireplace
				InventoryItem invItem = InventoryItem.Cast( source );
				EntityAI parent = invItem.GetHierarchyParent();
				if (parent && !parent.IsFireplace())
					parent = null;
				
				if (!DayZPhysics.RaycastRV(headPos, pos, contactPos, contactDir, contactComponent, NULL, player, parent, false, false, ObjIntersectFire))
				{
					if( MiscGameplayFunctions.IsPlayerOrientedTowardPos(player, pos, 60) )
					{
						visual = true;
					}
					player.OnPlayerReceiveFlashbangHitStart(visual);
				}
			}
		}
	}
	

	void FlashGrenade()
	{
		SetAmmoType("FlashGrenade_Ammo");
		SetFuseDelay(2);
		SetGrenadeType(EGrenadeType.FRAGMENTATION);
		SetParticleExplosion(ParticleList.GRENADE_M84);
	}

	void ~FlashGrenade() {}
}