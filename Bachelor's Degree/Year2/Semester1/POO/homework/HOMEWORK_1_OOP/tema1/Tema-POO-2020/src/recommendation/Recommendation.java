package recommendation;

import common.Constants;
import fileio.ActionInputData;
import fileio.Writer;
import newfileio.Input;
import org.json.simple.JSONArray;

import java.io.IOException;

@SuppressWarnings("unchecked")
public class Recommendation {
    private final JSONArray arrayResult;
    private final Input input;
    private final Writer fileWriter;

    public Recommendation(final JSONArray arrayResult, final Input input, final Writer fileWriter) {
        this.arrayResult = arrayResult;
        this.input = input;
        this.fileWriter = fileWriter;
    }

    /**
     * Solve the current recommendation
     * @param recommendation represents the current recommendation
     * @throws IOException in case of exceptions to reading / writing
     */
    public void solveRecommendation(final ActionInputData recommendation) throws IOException {
        String type = recommendation.getType();

        switch (type) {
            case Constants.STANDARD -> {
                StandardRecommendation stdRec = new StandardRecommendation(input, fileWriter);
                arrayResult.add(stdRec.solveRecommendation(recommendation));
            }
            case Constants.BESTUNSEEN -> {
                BestUnseenRecommendation bstUnsRec = new BestUnseenRecommendation(input,
                        fileWriter);
                arrayResult.add(bstUnsRec.solveRecommendation(recommendation));
            }
            case Constants.POPULAR -> {
                PopularRecommendation popRec = new PopularRecommendation(input, fileWriter);
                arrayResult.add(popRec.solveRecommendation(recommendation));
            }
            case Constants.SEARCH -> {
                SearchRecommendation searchRec = new SearchRecommendation(input, fileWriter);
                arrayResult.add(searchRec.solveRecommendation(recommendation));
            }
            case Constants.FAVORITE -> {
                FavoriteRecommendation favRec = new FavoriteRecommendation(input, fileWriter);
                arrayResult.add(favRec.solveRecommendation(recommendation));
            }
            default -> throw new IllegalStateException("Unexpected value: " + type);
        }
    }

}
