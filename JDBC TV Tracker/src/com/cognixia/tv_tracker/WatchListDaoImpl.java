package com.cognixia.tv_tracker;

import java.sql.Connection;
import java.sql.PreparedStatement;
import java.sql.ResultSet;
import java.sql.SQLException;
import java.sql.Statement;
import java.util.ArrayList;
import java.util.List;

import com.cognixia.connection.ConnectionManager;

public class WatchListDaoImpl implements WatchListDao{

	private Connection connection = null;
	
	@Override
	public void establishConnection() throws ClassNotFoundException, SQLException {
		
		if(connection == null) {
			connection = ConnectionManager.getConnection();
		}
	}

	@Override
	public void closeConnection() throws SQLException {
		connection.close();
	}

	//validates the username and password combo entered in by the user
	@Override
	public int validateUserPass(String userName, String pass) throws UserNotFoundException {
		
		//first verify if the username entered exists in the database, throw custom exception otherwise
		String sql = "SELECT * FROM tv_users WHERE tv_username = ?";
		try(PreparedStatement preparedStatement = connection.prepareStatement(sql)){
			preparedStatement.setString(1, userName);
			
			ResultSet resultSet = preparedStatement.executeQuery();
			if(resultSet.next() == false) {
				throw new UserNotFoundException(userName);
			}
		} catch (SQLException e1) {
			e1.printStackTrace();
		}
		
		//once the username is verified, check if they entered the right password
		sql = "SELECT * FROM tv_users WHERE tv_username = ? AND tv_password = ?";
		try(PreparedStatement preparedStatement = connection.prepareStatement(sql)){
			preparedStatement.setString(1, userName);
			preparedStatement.setString(2, pass);
			
			
			ResultSet resultSet = preparedStatement.executeQuery();
			String sqlUser = "";
			String sqlPass = "";
			
			//if the entry matching the username and password was found
			if(resultSet.next()) {
				sqlUser = resultSet.getString("tv_username");
				sqlPass = resultSet.getString("tv_password");
				
				//comparing case sensitivity in java
				if(userName.equals(sqlUser) && pass.equals(sqlPass)) {
					return resultSet.getInt("user_id");
				}
			}
			
		}
		catch(SQLException e){
			e.printStackTrace();
		}
		
		//returns -1 if the password is wrong
		return -1;
		
	}

	//adds a show to the user_shows table and returns true if the operation was successful, returns false otherwise
	@Override
	public boolean addShow(int userId, String showName) throws ShowNotFoundException {
		
		//first checks if the showName exists within the database, throws custom exception if it is not found
		String sql = "SELECT * FROM tv_shows WHERE tv_showname = ?"; 
		
		try(PreparedStatement preparedStatement = connection.prepareStatement(sql)){
			preparedStatement.setString(1, showName);
			
			ResultSet resultSet = preparedStatement.executeQuery();
			
			//if the show could not be found given the showName in the database
			if(resultSet.next() == false) {
				throw new ShowNotFoundException(showName);
			}
			
		} catch(SQLException e) {
			e.printStackTrace();
		}
		
		//once the show is verified, insert the entry into user_shows and return true
		sql = "INSERT INTO user_shows (user_id, show_id, show_status) " 
		+ "SELECT ?, tv_shows.show_id, 1 FROM tv_shows WHERE tv_shows.tv_showname = ?";
		
		try(PreparedStatement preparedStatement = connection.prepareStatement(sql)){
			preparedStatement.setInt(1, userId);
			preparedStatement.setString(2, showName);
			
			//If the rows affected is > 0, then the show was correctly inserted into the table
			if(preparedStatement.executeUpdate() > 0){
				return true;
			}
		} catch(SQLException e){
			e.printStackTrace();
		}
		
		//the show could not be inserted into the table
		return false;
	}
		
	

	@Override
	public boolean updateShowStatus(int userId, String showName, int status) {
		
		//starting from the junction table, joins the tv_shows and tv_users table to find the user id associated
		// with the showname and updates the status
		String sql = "UPDATE user_shows us "
				+ "INNER JOIN tv_users u ON us.user_id = u.user_id "
				+ "INNER JOIN tv_shows s ON us.show_id = s.show_id "
				+ "SET us.show_status = ? "
				+ "WHERE u.user_id = ? "
				+ "AND s.tv_showname = ?";
		
		try(PreparedStatement preparedStatement = connection.prepareStatement(sql)){
			preparedStatement.setInt(1, status);
			preparedStatement.setInt(2, userId);
			preparedStatement.setString(3, showName);
			
			//returns true if the query returns 1 (1 row affected)
			if(preparedStatement.executeUpdate() > 0){
				return true;
			}
			
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return false;
	}

	@Override
	public boolean deleteShow(int userId, String showName) {
		
		//deleting a show from user_shows table while referencing the tv_shows table to match the show_id's.
		//then finds the entry where the user_id and showname match
		String sql = "DELETE FROM user_shows "
					+ "WHERE user_id = ? AND show_id IN "
					+ "(SELECT show_id FROM tv_shows WHERE tv_showname = ?)";
		
		try (PreparedStatement preparedStatement = connection.prepareStatement(sql)) {
            preparedStatement.setInt(1, userId);
            preparedStatement.setString(2, showName);
            
          //returns true if the query returns 1 (1 row affected)
            if(preparedStatement.executeUpdate() > 0) {
            	return true;
            }
            
		} catch (SQLException e) {
            e.printStackTrace();
        }
		
		return false;
	}

	@Override
	public List<Shows> getAllMyShows(int userId) {

		//creating a list of shows to return as an arraylist
		List<Shows> showList = new ArrayList<>();
		
		String sql = "SELECT * FROM tv_shows INNER JOIN user_shows ON tv_shows.show_id = user_shows.show_id WHERE user_shows.user_id = ?";
		try(PreparedStatement preparedStatement = connection.prepareStatement(sql)) {	
			preparedStatement.setInt(1, userId);
			
			ResultSet resultSet = preparedStatement.executeQuery();
			
			while(resultSet.next()) {
				String showName = resultSet.getString("tv_showname");
				String showGenre = resultSet.getString("tv_genre");
				int showId = resultSet.getInt("show_id");
				
				
				Shows show = new Shows(showId, showName, showGenre);
				showList.add(show);
				
			}
		} catch (SQLException e) {
			e.printStackTrace();
		}
		return showList;
	}

	@Override
	public List<Shows> getAllMyShows(int userId, int status) {
		
		//creating a list of shows to return as an arraylist
		List<Shows> showList = new ArrayList<>();
		
		String sql = "SELECT s.show_id, s.tv_showname, s.tv_genre "
                + "FROM tv_shows s "
                + "JOIN user_shows us ON s.show_id = us.show_id "
                + "WHERE us.user_id = ? AND us.show_status = ?";
		
		try (PreparedStatement preparedStatement = connection.prepareStatement(sql)) {
			preparedStatement.setInt(1, userId);
            preparedStatement.setInt(2, status);
            
            ResultSet resultSet = preparedStatement.executeQuery(); 
            	
                while (resultSet.next()) {
                    String showName = resultSet.getString("tv_showname");
                    String genre = resultSet.getString("tv_genre");
                    int showId = resultSet.getInt("show_id");
                    
                    Shows show = new Shows(showId, showName, genre);
    				showList.add(show);     
                }
  
        } catch (SQLException e) {
            e.printStackTrace();
        }
		return showList;
	}

	@Override
	public List<Shows> getAllShows() {
		
		//creating a list of shows to return as an arraylist
		List<Shows> showList = new ArrayList<>();
		
		//
		try(Statement statement = connection.createStatement();
			ResultSet resultSet = statement.executeQuery("SELECT * FROM tv_shows")) {	
			
				while(resultSet.next()) {
					String showName = resultSet.getString("tv_showname");
					String showGenre = resultSet.getString("tv_genre");
					int showId = resultSet.getInt("show_id");
					
					Shows show = new Shows(showId, showName, showGenre);
					showList.add(show);
					
				}
			} catch (SQLException e) {
				e.printStackTrace();
			}
			return showList;
	}

	@Override
	public List<Shows> getAllShows(String genre) {
		
		//creating a list of shows to return as an arraylist
		List<Shows> showList = new ArrayList<>();
		
		//selecting all records of tv_shows within a certain genre
		String sql = "SELECT * FROM tv_shows WHERE tv_genre = ?";
		try(PreparedStatement preparedStatement = connection.prepareStatement(sql)) {	
			preparedStatement.setString(1, genre);
			
			ResultSet resultSet = preparedStatement.executeQuery();
			
				while(resultSet.next()) {
					String showName = resultSet.getString("tv_showname");
					String showGenre = resultSet.getString("tv_genre");
					int showId = resultSet.getInt("show_id");
					
					//creating a new show based on the current iteration of resultSet and adding it to the showList
					Shows show = new Shows(showId, showName, showGenre);
					showList.add(show);
					
				}
			} catch (SQLException e) {
				e.printStackTrace();
			}
			return showList;
	}
	
	public int getShowStatus(int userId, int showId) {
		
		int showStatus = -1;
		//
		String sql = "SELECT show_status FROM user_shows WHERE user_shows.user_id = ? AND user_shows.show_id = ?";
		try(PreparedStatement preparedStatement = connection.prepareStatement(sql)) {
			preparedStatement.setInt(1, userId);
			preparedStatement.setInt(2, showId);
			
			ResultSet resultSet = preparedStatement.executeQuery();
			
			if(resultSet.next()) {
				showStatus = resultSet.getInt("show_status");
			}
			
		} catch (SQLException e) {
			e.printStackTrace();
		}
		
		return showStatus;
		
	}

}
