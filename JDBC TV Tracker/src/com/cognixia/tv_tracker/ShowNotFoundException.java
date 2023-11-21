package com.cognixia.tv_tracker;

public class ShowNotFoundException extends Exception {
	
	/**
	 * for synchronization when calling the exception
	 */
	private static final long serialVersionUID = 1L;

	// construct exception to be thrown when shows is not found
	public ShowNotFoundException(String showName)
	{
		super("The show " + showName + " could not be found");
	}

}