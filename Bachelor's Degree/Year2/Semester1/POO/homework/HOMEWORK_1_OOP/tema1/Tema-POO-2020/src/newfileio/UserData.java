package newfileio;

import fileio.UserInputData;

import java.util.ArrayList;
import java.util.HashMap;
import java.util.List;
import java.util.Map;

public final class UserData {
    /**
     * User's username
     */
    private final String username;
    /**
     * Subscription Type
     */
    private final String subscriptionType;
    /**
     * The history of the movies seen
     */
    private final Map<String, Integer> history;
    /**
     * Movies added to favorites
     */
    private final ArrayList<String> favoriteMovies;
    /**
     * The history of accorded ratings
     */
    private final Map<String, Double> historyRatings;

    public UserData(final String username,
                    final String subscriptionType,
                    final Map<String, Integer> history,
                    final ArrayList<String> favoriteMovies) {
        this.username = username;
        this.subscriptionType = subscriptionType;
        this.favoriteMovies = favoriteMovies;
        this.history = history;
        this.historyRatings = new HashMap<>();
    }

    /**
     * Adds a rating map (from the String of the film rated to the grade given)
     * for every user in the input
     * @param users represents a list with the users in old format
     * @return a new list with a rating map for every user
     */
    public static List<UserData> addRatingMap(final List<UserInputData> users) {
        List<UserData> users2 = new ArrayList<>();
        for (UserInputData user : users) {
            UserData movieAux = new UserData(user.getUsername(), user.getSubscriptionType(),
                    user.getHistory(), user.getFavoriteMovies());
            users2.add(movieAux);
        }
        return users2;
    }

    public String getUsername() {
        return username;
    }

    public Map<String, Integer> getHistory() {
        return history;
    }

    public String getSubscriptionType() {
        return subscriptionType;
    }

    public ArrayList<String> getFavoriteMovies() {
        return favoriteMovies;
    }

    public Map<String, Double> getHistoryRatings() {
        return historyRatings;
    }

    @Override
    public String toString() {
        return "UserInputData{" + "username='"
                + username + '\'' + ", subscriptionType='"
                + subscriptionType + '\'' + ", history="
                + history + ", favoriteMovies="
                + favoriteMovies + '}';
    }

}
