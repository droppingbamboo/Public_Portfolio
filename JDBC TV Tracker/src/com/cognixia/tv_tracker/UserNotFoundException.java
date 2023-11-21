package com.cognixia.tv_tracker;

public class UserNotFoundException extends Exception {
	
	/**
	 * for synchronization when calling the exception
	 */
	private static final long serialVersionUID = 1L;

	//construct the exception that is thrown when user is not found
	public UserNotFoundException(String userName) {
		
		super("User " + userName + " could not be found");
	}

}
