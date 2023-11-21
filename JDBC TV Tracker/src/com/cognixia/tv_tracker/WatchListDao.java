package com.cognixia.tv_tracker;

import java.sql.SQLException;
import java.util.List;


public interface WatchListDao {
		// needed for later so we make sure that the connection manager gets called
		public void establishConnection() throws ClassNotFoundException, SQLException;
		
		// as well, this method will help with closing the connection
		public void closeConnection() throws SQLException ;
		
		
		public int validateUserPass(String userName, String pass) throws UserNotFoundException;
		
		public boolean addShow(int userId, String showName) throws ShowNotFoundException;
		
		public boolean updateShowStatus(int userId, String showName, int status);
		
		public boolean deleteShow(int userId, String showName);
		
		public List<Shows> getAllMyShows(int userId);
		
		public List<Shows> getAllMyShows(int userId, int status);
		
		public List<Shows> getAllShows();
		
		public List<Shows> getAllShows(String genre);
		
		public int getShowStatus(int userId, int showId);
		
}
