/**@class	CargoBase
 * @brief	represents base for cargo storage for entities
 *
 * @NOTE: rows == y axis
 **/
class CargoBase : Managed
{
	/**@fn			GetCargoOwner
	 * @brief		get the entity that owns the cargo
	 * @return		cargo owner
	 **/
	proto native EntityAI GetCargoOwner ();
	
	/**@fn			GetItemCount
	 * @return		number of items in cargo
	 **/
	proto native int GetItemCount ();
	/**@fn			GetItem
	 * @return		get item at specific index
	 **/
	proto native EntityAI GetItem (int index);

	/**@fn			GetWidth
	 * @return		width of the cargo
	 **/
	proto native int GetWidth ();
	/**@fn			GetHeight
	 * @return		height of the cargo
	 **/
	proto native int GetHeight ();
	/**@fn			GetItemRowCol
	 * @param[out]	row			returned row of the item at internal index
	 * @param[out]	col			returned col. 0 on xbox
	 **/
	proto void GetItemRowCol (int index, out int row, out int col);
	/**@fn			GetItemSize
	 * @param[out]	w			returned width of the item at internal index
	 * @param[out]	h			returned height of the item at internal index
	 **/
	proto void GetItemSize (int index, out int w, out int h);

	/**@fn			FindEntityInCargo
	 * @return		find internal index of the entity in cargo or -1 if not found
	 **/
	proto native int FindEntityInCargo (notnull EntityAI e);

	private void CargoBase ();
	private void ~CargoBase ();
	
	/**@fn			CanReceiveItemIntoCargo
	 * @brief		condition EntityAI::CanReceiveItemIntoCargo for Cargo.
	 * @return		true if cargo can be added, false otherwise
	 **/
	bool CanReceiveItemIntoCargo (EntityAI item) { return true; }
};


#ifdef PLATFORM_CONSOLE
	class CargoList : CargoBase
	{
		/**@fn			GetMaxWeight
		 * @return		maximum weight that the cargo can hold
		 **/
		proto native int GetMaxWeight ();
	
		/**@fn			GetTotalWeight
		 * @brief		sums weight of all items in cargo and adds weight of item if item != null
		 * @return		sum of weights plus weight of item (if !null)
		 **/
		proto native int GetTotalWeight (EntityAI item);

		/**@fn			CanFitItemIntoCargo
		 * @return		true if adding item does not exceed GetMaxWeight, false otherwise
		 **/		
		proto native bool CanFitItemIntoCargo (EntityAI cargo);
	
		/**@fn			CanReceiveItemIntoCargo
		 * @return		true if adding item does not exceed GetMaxWeight, false otherwise
		 **/
		override bool CanReceiveItemIntoCargo (EntityAI item)
		{
			return CanFitItemIntoCargo(item);
		}
	};
#else
	class CargoGrid : CargoBase
	{
		/**@fn			FindEntityInCargoOn
		 * @return		get item at coordinates (row, col). col is 0 on xbox.
		 **/
		proto native EntityAI FindEntityInCargoOn (int row, int col);
	};
#endif
