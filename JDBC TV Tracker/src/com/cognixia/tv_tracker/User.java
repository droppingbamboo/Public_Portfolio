package com.cognixia.tv_tracker;


public class User {
	
	// User's Attributes 
	
	private int userId; // holds the user's id
	
	private String userName; // holds the user's name
	
	private String password; // holds the user's password
	
	//User's Methods
	
	//constructs user object
	public User(int userId, String userName, String password)
	{
		this.userId = userId;
		this.userName = userName;
		this.password = password;
	}
	
	//get user's name method
	public String getUserName() {
		return userName;
	}
	
	//get user's password method
	public String getPassword() {
		return password;
	}
	
	//get user's id method
	public int getUserId()
	{
		return userId;
	}
	
	

	

}

