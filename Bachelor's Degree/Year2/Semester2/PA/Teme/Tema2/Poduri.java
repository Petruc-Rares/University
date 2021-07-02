
import java.io.FileWriter;
import java.io.IOException;
import java.util.LinkedList;
import java.util.Queue;

class Poduri {

	private static class SeaGrid {
		private Integer N;
		// used for further calculations
		private Integer totalSize;
		private Integer extendedN;
		private Integer extendedM;
		private Integer M;
		private Integer idStart;
		private Integer idxRowStart;
		private Integer idxColumnStart;
		private char[][] adj;

		SeaGrid(Integer N, Integer M) {
			this.N = N;
			this.M = M;
			this.totalSize = M * N + 2 * (M + N) + 4;
			this.extendedN = N + 2;
			this.extendedM = M + 2;
			adj = new char[N][M];
		}

		public Integer getTotalSize() {
			return totalSize;
		}

		public char[][] getAdj() {
			return adj;
		}

		public void setIdxRowStart(Integer idxRowStart) {
			this.idxRowStart = idxRowStart;
		}

		public Integer getIdxRowStart() {
			return idxRowStart;
		}

		public Integer getIdxColumnStart() {
			return idxColumnStart;
		}

		public void setIdxColumnStart(Integer idxColumnStart) {
			this.idxColumnStart = idxColumnStart;
		}

		public Integer getCorrespondingIdX(Integer RowIdx, Integer ColumnIdx) {
			return RowIdx * extendedM + ColumnIdx;
		}

		public Integer getIdStart() {
			return idStart;
		}

		public void setIdStart(Integer idStart) {
			this.idStart = idStart;
		}
	}

	static class IntPair {
		int x;
		int y;
		int id;

		IntPair(int x, int y, int id) {
			this.x = x;
			this.y = y;
			this.id = id;
		}
	}


	static Integer getMinimumDistance(SeaGrid seaGrid) {
		Integer totalSize = seaGrid.getTotalSize();
		int[] distances = new int[totalSize];
		boolean[] visited = new boolean[totalSize];
		Queue<IntPair> queue = new LinkedList<>();

		Integer src = seaGrid.idStart;
		distances[src] = 0;
		queue.add(new IntPair(seaGrid.getIdxColumnStart(), seaGrid.getIdxRowStart(), src));

		Integer marginDown = seaGrid.N - 1;
		Integer marginRight = seaGrid.M - 1;

		while (!queue.isEmpty()) {
			IntPair crtElem = queue.remove();
			Integer crtRow = crtElem.y;
			Integer crtColumn = crtElem.x;

			Integer crtId = crtElem.id;
			if (visited[crtId] == false) {
				visited[crtId] = true;
				
				char crtChar = seaGrid.adj[crtRow][crtColumn];
				// check the case the crtChar matches
				if (crtChar == '.') {
					continue;
				} else if (crtChar == 'V') {
					if ((crtRow == 0) || (crtRow.equals(marginDown))) {
						return distances[crtId] + 1;
					}
					Integer newDistance = distances[crtId] + 1;
					
					// add the North and the South coordinates
					queue.add(new IntPair(crtColumn, crtRow - 1, crtId - seaGrid.extendedM));
					queue.add(new IntPair(crtColumn, crtRow + 1, crtId + seaGrid.extendedM));

					distances[crtId - seaGrid.extendedM] = newDistance;
					distances[crtId + seaGrid.extendedM] = newDistance;
				} else if (crtChar == 'O') {
					if ((crtColumn == 0) || (crtColumn.equals(marginRight))) {
						return distances[crtId] + 1;
					}
					Integer newDistance = distances[crtId] + 1;
					// add the West and the East coordinates
					queue.add(new IntPair(crtColumn - 1, crtRow, crtId - 1));
					queue.add(new IntPair(crtColumn + 1, crtRow, crtId + 1));

					distances[crtId - 1] = newDistance;
					distances[crtId + 1] = newDistance;
				} else if (crtChar == 'D') {
					if ((crtColumn == 0) || (crtColumn.equals(marginRight))
							|| (crtRow == 0) || (crtRow.equals(marginDown))) {
						return distances[crtId] + 1;
					}
					// add all the coordinates
					queue.add(new IntPair(crtColumn, crtRow - 1, crtId - seaGrid.extendedM));
					queue.add(new IntPair(crtColumn, crtRow + 1, crtId + seaGrid.extendedM));
					queue.add(new IntPair(crtColumn - 1, crtRow, crtId - 1));
					queue.add(new IntPair(crtColumn + 1, crtRow, crtId + 1));

					Integer newDistance = distances[crtId] + 1;
					distances[crtId - seaGrid.extendedM] = newDistance;
					distances[crtId + seaGrid.extendedM] = newDistance;
					distances[crtId - 1] = newDistance;
					distances[crtId + 1] = newDistance;
				}
			}
		}

		return -1;
	}

	public static SeaGrid createGrid() {
		MyScanner scanner = new MyScanner("poduri.in");

		Integer N = scanner.nextInt();
		Integer M = scanner.nextInt();

		SeaGrid seaGrid = new SeaGrid(N, M);

		Integer startPosY = scanner.nextInt();
		Integer startPosX = scanner.nextInt();

		seaGrid.setIdxRowStart(startPosY - 1);
		seaGrid.setIdxColumnStart(startPosX - 1);

		seaGrid.setIdStart(seaGrid
				.getCorrespondingIdX(startPosY - 1, startPosX - 1));

		String line;

		char[][] seaGridRepr = seaGrid.getAdj();

		int idxRow = 0;
		while ((line  = scanner.nextLine()) != null) {
			seaGridRepr[idxRow++] =  line.toCharArray();
		}

		return seaGrid;
	}

	public static void main(String[] args) throws IOException {
		SeaGrid seaGrid = createGrid();

		Integer minimumDistance = getMinimumDistance(seaGrid);

		try {
			FileWriter myWriter = new FileWriter("poduri.out");
			myWriter.write(minimumDistance + "");
			myWriter.close();
		} catch (IOException e) {
			System.out.println("An error occurred.");
			e.printStackTrace();
		}
	}
}