///@{ states

enum HandStateID
{
	UNKNOWN				=  0,
	Empty				=  1,
	Equipped			=  2,
}

class HandStateEmpty : HandStableState
{
	override void OnEntry (HandEventBase e) { super.OnEntry(e); }
	override void OnExit (HandEventBase e) { super.OnExit(e); }
	override int GetCurrentStateID () { return HandStateID.Empty; }
};

class HandStateEquipped : HandStableState
{
	override void OnEntry (HandEventBase e) { super.OnEntry(e); }
	override void OnExit (HandEventBase e) { super.OnExit(e); }
	override int GetCurrentStateID () { return HandStateID.Equipped; }
};

///@} states

