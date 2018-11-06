enum DayZInfectedConstants
{
	//! anim commands
	COMMANDID_MOVE,
	COMMANDID_VAULT,
	COMMANDID_DEATH,
	COMMANDID_HIT,
	COMMANDID_ATTACK,
	COMMANDID_CRAWL,
	
	//! mind states
	MINDSTATE_CALM,
	MINDSTATE_DISTURBED,
	MINDSTATE_ALERTED,
	MINDSTATE_CHASE,
	MINDSTATE_FIGHT,
};

enum DayZInfectedConstantsMovement
{
	MOVEMENTSTATE_IDLE = 0,
	MOVEMENTSTATE_WALK,
	MOVEMENTSTATE_RUN,
	MOVEMENTSTATE_SPRINT
}

class DayZInfectedCommandMove
{
	proto native void SetStanceVariation(int pStanceVariation);
	proto native void SetIdleState(int pIdleState);
	proto native bool IsTurning();
}

class DayZInfectedCommandVault
{
	proto native bool WasLand();
}

class DayZInfectedCommandAttack
{
	proto native bool WasHit();
}

class DayZInfected extends DayZCreatureAI
{	
	proto native DayZInfectedType GetDayZInfectedType();
	proto native DayZInfectedInputController GetInputController();	
	proto native DayZInfectedCommandMove StartCommand_Move();	
	proto native DayZInfectedCommandVault StartCommand_Vault(int pType);	
	proto native void StartCommand_Death(int pType, float pDirection);
	proto native void StartCommand_Hit(bool pHeavy, int pType, float pDirection);
	proto native DayZInfectedCommandAttack StartCommand_Attack(EntityAI pTarget, int pType, float pSubtype);
	proto native void StartCommand_Crawl(int pType);
	
	proto native bool CanAttackToPosition(vector pTargetPosition);
	
	proto native DayZInfectedCommandMove GetCommand_Move();
	proto native DayZInfectedCommandVault GetCommand_Vault();
	proto native DayZInfectedCommandAttack GetCommand_Attack();
		
	//-------------------------------------------------------------
	void DayZInfected()
	{
	}
	
	//-------------------------------------------------------------
	void ~DayZInfected()
	{
	}
	
	//-------------------------------------------------------------
}