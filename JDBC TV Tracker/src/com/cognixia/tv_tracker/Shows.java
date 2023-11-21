package com.cognixia.tv_tracker;

public class Shows {
	
	//attributes
	private int showId; // holds the id of show
	
	private String showName; //holds the name of show
	
	private String genre; // holds the genre of show
	
	//methods
	
	//constructs a show
	public Shows(int showId, String showName, String genre) {
		super();
		this.showId = showId;
		this.showName = showName;
		this.genre = genre;
	}


	//get show's id
	public int getShowId() {
		return showId;
	}
	
	//get show's name
	public String getShowName() {
		return showName;
	}


	//get genre's name
	public String getGenre() {
		return genre;
	}
	
	//converts shows into a String
	@Override
	public String toString() {
		return "Shows [showId=" + showId + ", showName=" + showName + ", genre=" + genre + "]";
	}
}
