{-# OPTIONS_GHC -Wall #-}
{-# LANGUAGE EmptyDataDecls, MultiParamTypeClasses,
             TypeSynonymInstances, FlexibleInstances,
             InstanceSigs #-}

module Basics where
{-
    Expune funcțiile necesare reprezentării jocului.
-}

import ProblemState
import Data.List
import Data.Maybe

{-
    Sinonim tip de date pentru reprezetarea unei perechi (Int, Int)
    care va reține coordonatele celulelor de pe tabla de joc.
    Colțul stânga-sus este (0, 0).
-}
type Position = (Int, Int)

{-
    Tip de date pentru reprezentarea Target-urilor.
    Acestea conțin informații atât despre poziția curentă a
    Target-ului cât și despre comportamentul acestuia.
    Tipul Behavior este definit mai jos.
-}
data Target = Target {
    position :: Position,
    behavior :: Behavior
}

instance Show Target where
    show target = show (fst (position target)) ++ show (snd (position target))

instance Eq Target where
    Target p1 _ == Target p2 _ = p1 == p2

instance Ord Target where
    Target p1 _ <= Target p2 _ = p1 <= p2

{-
    Tip de date pentru reprezentarea comportamentului unui Target.
    Tipul Behavior este utilizat pentru a modela tranziția Target-urilor
    din starea curentă în starea următoare. Primul parametru este poziția
    actuală a target-ului, iar al doilea, starea curentă a jocului.
    Tipul Game este definit mai jos.
    
    Observați că, din moment ce un Behavior produce un Target nou,
    acesta din urmă ar putea fi caracterizat de un alt Behavior
    decât cel anterior.
-}
type Behavior = Position -> Game -> Target

{-
    Direcțiile de deplasare pe tablă
-}
data Direction = North | South | West | East
    deriving (Eq, Show)


obstaclesRepresentation :: [Char]
obstaclesRepresentation = '@':obstaclesRepresentation

freeSpacesRepresentation :: [Char]
freeSpacesRepresentation = ' ':freeSpacesRepresentation
{-
    *** TODO ***
    
    Tip de date pentru reprezentarea stării jocului, la un anumit
    moment. Completați-l cu orice informație aveți nevoie pentru
    stocarea stării jocului (hunter, target, obstacole, gateways).
-}
data Game = Game
    { noLines    :: Int
    , noColumns     :: Int
    , hunter    :: Position
    , targets   :: [Target]
    , obstacles :: [Position]
    , gateways  :: [(Position, Position)]
    } deriving (Eq, Ord)
{-
    *** Optional *** 
  
    Dacă aveți nevoie de o funcționalitate particulară,
    instantiați explicit clasele Eq și Ord pentru Game.
    În cazul acesta, eliminați deriving (Eq, Ord) din Game.
-}


--{-

fillPlayableRows:: Int -> Int -> [String]
fillPlayableRows noLines noColumns = concatMap (\crtLine -> [lineRepr ++ "\n"]) [0..(noLines-3)]
                             where
                                lineRepr = take 1 obstaclesRepresentation ++ take (noColumns - 2) freeSpacesRepresentation ++ take 1 obstaclesRepresentation

fillTableLimits :: [String] -> Int -> [String]
fillTableLimits table noColumns = enclosedTable
    where
        enclosedTable = [obstaclesLine ++ "\n"] ++ table ++ [obstaclesLine]
        obstaclesLine = take noColumns obstaclesRepresentation
        
fillTargets :: [String] -> [Target] -> [String]
fillTargets crtTable targets = foldl (\table crtTarget -> updateMatrix table '*' (position crtTarget)) crtTable targets

fillObstacles :: [String] -> [Position] -> [String]
fillObstacles crtTable obstacles = foldl (\table crtObstacle -> updateMatrix table '@' crtObstacle) crtTable obstacles

fillGateways :: [String] -> [(Position, Position)] -> [String]
fillGateways crtTable gateways = foldl (\table crtGateway -> updateMatrix table '#' crtGateway) crtTable $ concatMap (\pair -> [fst pair, snd pair]) gateways

updateMatrix :: [[a]] -> a -> (Int, Int) -> [[a]]
updateMatrix matrix elem (rowIdx, columnIdx) =
    updatedMatrix
    where
        updatedMatrix = take rowIdx matrix ++
                       [take columnIdx (matrix !! rowIdx) ++ [elem] ++ drop (columnIdx + 1) (matrix !! rowIdx)] ++
                       drop (rowIdx + 1) matrix

---}

{-
    *** TODO ***

    Reprezentați starea jocului ca șir de caractere, pentru afișarea
    la consolă.
    
    Atenție! Fiecare linie, mai puțin ultima, este urmată de \n.
    Celule goale vor fi reprezentate ca ' '.
    Hunter-ul va fi reprezentat ca '!'.
    Target-urile vor fi reprezentate ca '*'
    Gateways-urile vor fi reprezentate ca '#'.
    Obstacolele vor fi reprezentate de '@'.

    Hint: S-ar putea să vă fie utile list comprehensions,
    precum și funcțiile elem, any și intercalate din Data.List.
-}




gameAsString :: Game -> String
gameAsString game = 
    defaultGame
        where
            defaultGamePlayable = fillPlayableRows (noLines game) (noColumns game)
            defaultGameEnclosed = fillTableLimits defaultGamePlayable (noColumns game)
            defaultGameObstacles = fillObstacles defaultGameEnclosed (obstacles game)
            defaultGameGateways = fillGateways defaultGameObstacles (gateways game)         
            defaultGameTargets = fillTargets defaultGameGateways (targets game)      
            defaultGameHunter = updateMatrix defaultGameTargets '!' (hunter game)
            defaultGame = concat $ defaultGameHunter

instance Show Game where
    show = gameAsString

{-
    *** TODO ***
    
    Primește numărul de linii și numărul de coloane ale tablei de joc.
    Intoarce un obiect de tip Game în care tabla conține spații goale în interior, fiind
    împrejmuită de obstacole pe toate laturile. Implicit, colțul din stânga sus este (0,0),
    iar Hunterul se găsește pe poziția (1, 1).
-}

{-
data Game = Game
    { length    :: Int
    , noColumns     :: Int
    , hunter    :: Position
    , targets   :: [Position]
    , obstacles :: [Position]
    , gateways  :: [Position]
    } deriving (Eq, Ord)
-}

emptyGame :: Int -> Int -> Game
emptyGame noLines noColumns =
    Game noLines noColumns defaultHunter defaultTarget defaultObstacles defaultGateways
    where
        defaultHunter = (1, 1)
        defaultTarget = []
        defaultObstacles = [(i, j) | i <- [0..(noLines - 1)], j <- [0..(noColumns - 1)], i==0 || j==0 || i==(noLines-1) || j==(noColumns-1)]
        defaultGateways = []

{-
    *** TODO ***

    Primește o poziție și un joc și întoarce un nou joc, cu Hunter-ul pus
    pe poziția specificată.
    Parametrul Position reprezintă poziția de pe hartă la care va fi adaugat Hunter-ul
    Daca poziția este invalidă (ocupată sau în afara tablei de joc) se va întoarce
    același joc.
-}
addHunter :: Position -> Game -> Game
addHunter (rowIdx, columnIdx) prevGame = 
    Game (noLines prevGame) (noColumns prevGame) hunterPos (targets prevGame) (obstacles prevGame) (gateways prevGame)
    where
        hunterPos
            | rowIdx > 0 && rowIdx < ((noLines prevGame) - 1) && columnIdx> 0 && columnIdx < ((noColumns prevGame) - 1) = (rowIdx, columnIdx)
            | otherwise = (1, 1)

{-
    *** TODO ***

    Primește un comportament, o poziție și un joc și întoarce un nou joc, în care a fost
    adăugat Target-ul descris de comportament și poziție.
    Parametrul Behavior reprezintă comportamentul Hunter-ului care va fi adăugat.
    Parametrul Position reprezintă poziția de pe hartă la care va fi adăugat Target-ul.
-}
addTarget :: Behavior -> Position -> Game -> Game
addTarget behav (rowIdx, columnIdx) prevGame = 
    Game (noLines prevGame) (noColumns prevGame) (hunter prevGame) updatedTargets (obstacles prevGame) (gateways prevGame)
    where
        updatedTargets = (targets prevGame) ++
            case elem (rowIdx, columnIdx) (concatMap (\target -> [position target]) (targets prevGame)) == True of
            True -> []
            _ -> [Target (rowIdx, columnIdx) behav]



{-
    *** TODO ***

    Primește o pereche de poziții și un joc și întoarce un nou joc, în care au fost adăugate
    cele două gateway-uri interconectate.
    Parametrul (Position, Position) reprezintă pozițiile de pe hartă la care vor fi adăugate 
    cele două gateway-uri interconectate printr-un canal bidirecțional.
-}
addGateway :: (Position, Position) -> Game -> Game
addGateway ((rowIdx1, columnIdx1), (rowIdx2, columnIdx2)) prevGame =
    Game (noLines prevGame) (noColumns prevGame) (hunter prevGame) (targets prevGame) (obstacles prevGame) updatedGateways
    where
        updatedGateways = (gateways prevGame) ++ [((rowIdx1, columnIdx1), (rowIdx2, columnIdx2))]

{-
    *** TODO ***

    Primește o poziție și un joc și întoarce un nou joc, în care a fost adăugat un obstacol
    la poziția specificată.
    Parametrul Position reprezintă poziția de pe hartă la care va fi adăugat obstacolul.
-}
addObstacle :: Position -> Game -> Game
addObstacle (rowIdx, columnIdx) prevGame  = 
    Game (noLines prevGame) (noColumns prevGame) (hunter prevGame) (targets prevGame) updatedObstacles (gateways prevGame)
    where
        updatedObstacles = [(rowIdx, columnIdx)] ++ (obstacles prevGame)


-- return true if gateway exists
checkGateway :: Position -> [(Position, Position)] -> Bool
checkGateway _ [] = False
checkGateway (rowIdx, columnIdx) (((x, y), (z, t)):xs) = elem (rowIdx, columnIdx) [(x, y), (z, t)] || checkGateway (rowIdx, columnIdx) xs

findPairGateway :: Position  -> [(Position, Position)] -> (Position, Position)
findPairGateway (rowIdx, columnIdx) pairs = head(filter (\pair -> ((rowIdx, columnIdx) == (fst pair)) || ((rowIdx, columnIdx) == (snd pair)) ) pairs)

findPairedGateway :: Position -> [(Position, Position)] -> Maybe Position
findPairedGateway (rowIdx, columnIdx) pairs
    | (fst pairGateway) == (rowIdx, columnIdx) = Just $ (snd pairGateway)
    | otherwise = Just $ (fst pairGateway)
    where pairGateway = findPairGateway (rowIdx, columnIdx) pairs

-- return true if obstacle exists
checkObstacle :: Position -> [Position] -> Bool
checkObstacle _ [] = False
checkObstacle (rowIdx, columnIdx) ((x, y): xs) = (rowIdx, columnIdx) == (x, y) || checkObstacle (rowIdx, columnIdx)  xs


{-
    *** TODO ***
    
    Primește o poziție destinație înspre care vrea să se deplaseze o entitate (Hunter sau Target)
    și verifică daca deplasarea este posibilă, întorcând noua poziție, luând în considerare
    și Gateway-urile.
    Avem următoarele cazuri:
    - dacă poziția corespunde unui spațiu gol, se întoarce acea poziție;
    - dacă poziția corespunde unui gateway, se întoarce poziția gateway-ului pereche;
    - dacă poziția corespunde unui obstacol, se întoarce Nothing.
    Parametrul Position reprezintă poziția destinație.
-}
attemptMove :: Position -> Game -> Maybe Position
attemptMove (rowIdx, columnIdx) game
    -- verificam daca e obstacol
    | checkObstacle (rowIdx, columnIdx) (obstacles game) = Nothing
    -- verificam daca e gateway
    | checkGateway (rowIdx, columnIdx) (gateways game) = findPairedGateway (rowIdx, columnIdx) (gateways game)
    | otherwise = Just (rowIdx, columnIdx)
{-
    *** TODO ***

    Comportamentul unui Target de a se deplasa cu o casuță înspre est. 
    Miscarea se poate face doar daca poziția este validă (se află pe tabla de
    joc) și nu este ocupată de un obstacol. In caz contrar, Target-ul va rămâne 
    pe loc.
    
    Conform definiției, tipul Behavior corespunde tipului funcție
    Position -> Game -> Target.
    
    Având în vedere că cele patru funcții definite în continuare (goEast, goWest,
    goNorth, goSouth) sunt foarte similare, încercați să implementați o funcție
    mai generală, pe baza căreia să le definiți apoi pe acestea patru.
-}

-- Exemplu de point-free ***


add1 :: Num a => a -> a
add1 = (+1) . (*2)

goEast :: Behavior
goEast (rowIdx, columnIdx) game =
    newTarget
    where
        newTarget = case resultAttemptMove of
            -- need to check whether we sit on an obstacle or not
            Nothing -> getTarget2Cases (rowIdx, columnIdx) game goEast
            -- if gateway or if free space just move to That position
            _ -> Target (fromJust (resultAttemptMove)) goEast
            where
            resultAttemptMove = attemptMove (rowIdx, columnIdx + 1) game
            

{-
    *** TODO ***

    Comportamentul unui Target de a se deplasa cu o casuță înspre vest. 
    Miscarea se poate face doar daca poziția este validă (se află pe tabla de
    joc) și nu este ocupată de un obstacol. In caz contrar, Target-ul va rămâne 
    pe loc.
-}
goWest :: Behavior
goWest (rowIdx, columnIdx) game  =
    newTarget
    where
        newTarget = case resultAttemptMove of
            -- need to check whether we sit on an obstacle or not
            Nothing -> getTarget2Cases (rowIdx, columnIdx) game goWest
            -- if gateway or if free space just move to That position
            _ -> Target (fromJust (resultAttemptMove)) goWest
            where
            resultAttemptMove = attemptMove (rowIdx, columnIdx - 1) game

{-
    *** TODO ***

    Comportamentul unui Target de a se deplasa cu o casuță înspre nord. 
    Miscarea se poate face doar daca poziția este validă (se află pe tabla de
    joc) și nu este ocupată de un obstacol. In caz contrar, Target-ul va rămâne 
    pe loc.
-}
goNorth :: Behavior
goNorth (rowIdx, columnIdx) game =
    newTarget
    where
        newTarget = case resultAttemptMove of
            -- need to check whether we sit on an obstacle or not
            Nothing -> getTarget2Cases (rowIdx, columnIdx) game goNorth
            -- if gateway or if free space just move to That position
            _ -> Target (fromJust (resultAttemptMove)) goNorth
            where
            resultAttemptMove = attemptMove (rowIdx - 1, columnIdx) game
            
    {-       
    -- targetul nu trebuie sa ajunga pe prima linie (unde sunt obstacole)
    | rowIdx == 1 = Target (rowIdx, columnIdx) goNorth
    -- targetul nu trebuie sa se loveasca de vreun obstacol
    | elem (rowIdx - 1, columnIdx) (obstacles game) = Target (rowIdx, columnIdx) goNorth
    -- altfel, pozitia e valida
    | otherwise = Target (rowIdx - 1, columnIdx) goNorth
    -}

{-
    *** TODO ***

    Comportamentul unui Target de a se deplasa cu o casuță înspre sud. 
    Miscarea se poate face doar daca poziția este validă (se află pe tabla de
    joc) și nu este ocupată de un obstacol. In caz contrar, Target-ul va rămâne 
    pe loc.
    
    
    te folosesti de functia asta:
    attemptMove :: Position -> Game -> Maybe Position
    attemptMove (rowIdx, columnIdx) game
    -- verificam daca e gateway
    | checkObstacle (rowIdx, columnIdx) (obstacles game) = Nothing
    | checkGateway (rowIdx, columnIdx) (gateways game) = findPairedGateway (rowIdx, columnIdx) (gateways game)
    | otherwise = Just (rowIdx, columnIdx)
    
-}

getTarget2Cases :: Position -> Game -> Behavior -> Target
getTarget2Cases (rowIdx, columnIdx) game behav =
    finalTarget
    where
        finalTarget = case checkGateway (rowIdx, columnIdx) (gateways game) of
            -- if it is not a gatewayy
            False -> Target (rowIdx, columnIdx) behav
            _ -> Target (fromJust (findPairedGateway (rowIdx, columnIdx) (gateways game))) behav

goSouth :: Behavior
goSouth (rowIdx, columnIdx) game = 
    newTarget
    where
        newTarget = case resultAttemptMove of
            -- need to check whether we sit on an obstacle or not
            Nothing -> getTarget2Cases (rowIdx, columnIdx) game goSouth
            -- if gateway or if free space just move to That position
            _ -> Target (fromJust (resultAttemptMove)) goSouth
            where
            resultAttemptMove = attemptMove (rowIdx + 1, columnIdx) game

{-
    *** TODO ***

    Comportamentul unui Target de a-și oscila mișcarea, când înspre nord, când înspre sud. 
    Mișcarea se poate face doar dacă poziția este validă (se află pe tablă de
    joc) și nu este ocupată de un obstacol. In caz contrar, Target-ul iși va schimba
    direcția de mers astfel:
    - daca mergea inspre nord, își va modifica direcția miscării înspre sud;
    - daca mergea inspre sud, își va continua mișcarea înspre nord.
    Daca Target-ul întâlneste un Gateway pe traseul său, va trece prin acesta,
    către Gateway-ul pereche conectat și își va continua mișcarea în același sens la ieșire
    din acesta.
    Puteți folosit parametrul Int pentru a surprinde deplasamentul Target-ului (de exemplu,
    1 pentru sud, -1 pentru nord).
-}

tryBounceNorth :: Position -> Game -> Target
tryBounceNorth (rowIdx, columnIdx) game =
    newTarget
        where
            nextMove = attemptMove (rowIdx - 1, columnIdx) game
            newTarget = case nextMove of
                -- daca e obstacol
                Nothing -> Target (rowIdx + 1, columnIdx) $ bounce 1
                -- daca e spatiu liber sau gateway
                _ ->  Target (fromJust nextMove) $ bounce (-1)


tryBounceSouth :: Position -> Game -> Target
tryBounceSouth (rowIdx, columnIdx) game =
    newTarget
        where
            nextMove = attemptMove (rowIdx + 1, columnIdx) game
            newTarget = case nextMove of
                -- daca e obstacol
                Nothing -> Target (rowIdx - 1, columnIdx) $ bounce (-1)
                -- daca e spatiu liber sau gateway
                _ ->  Target (fromJust nextMove) $ bounce 1

bounce :: Int -> Behavior
bounce direction pos game = 
    newTarget
        where 
            newTarget = case direction of
                1 -> tryBounceSouth pos game
                -1 -> tryBounceNorth pos game

getNewTargetsPositions :: [Target] -> Game -> [Target]
getNewTargetsPositions prevTargets game = foldl (\newTargets crtTarget -> ((behavior crtTarget) (position crtTarget) game):newTargets) [] prevTargets

{-
    *** TODO ***
    Funcție care mută toate Target-urile din Game-ul dat o poziție, în functie
    de behavior-ul fiecăreia și întoarce noul Game în care pozițiile Target-urilor
    sunt actualizate.
    
-}
moveTargets :: Game -> Game
moveTargets game = 
    updatedTargetsGame
    where
        prevTargets = (targets game)
        -- first delete the prevTargets
        deletedPrevTargetsGame = Game (noLines game) (noColumns game) (hunter game) [] (obstacles game) (gateways game)
        -- then update the prevTargets to the new ones corresponding to their behavior
        updatedTargetsGame = Game (noLines game) (noColumns game) (hunter game) newTargets (obstacles game) (gateways game)
        newTargets = getNewTargetsPositions prevTargets game

{-
    *** TODO ***

    Verifică dacă Targetul va fi eliminat de Hunter.
    Un Target este eliminat de Hunter daca se află pe o poziție adiacentă
    cu acesta.
    Parametrul Position reprezintă poziția Hunterului pe tabla
    de joc.
    Parametrul Target reprezintă Targetul pentru care se face verificarea.
-}
isTargetKilled :: Position -> Target -> Bool
isTargetKilled (rH, cH) target
    | (rH == rT) && (cH == cT + 1) = True
    | (rH == rT) && (cH == cT - 1) = True
    | (cH == cT) && (rH == rT - 1) = True
    | (cH == cT) && (rH == rT + 1) = True
    | otherwise = False
    where
        posTarget = (position target)
        rT = (fst posTarget)
        cT = (snd posTarget)


{-
    *** TODO ***

    Avansează starea jocului curent, rezultând starea următoare a jocului.
    Parametrul Direction reprezintă direcția în care se va deplasa Hunter-ul.
    Parametrul Bool specifică dacă, după mutarea Hunter-ului, vor fi
    mutate și Target-urile sau nu, și dacă vor fi eliminate din joc sau nu.
    Este folosit pentru a distinge între desfășurarea reală a jocului (True)
    și planificarea „imaginată” de hunter (False) în partea a doua a temei.

    Avansarea stării jocului respectă următoarea ordine:
    1. Se deplasează Hunter-ul.
    2. În funcție de parametrul Bool, se elimină Target-urile omorâte de către Hunter.
    3. In funcție de parametrul Bool, se deplasează Target-urile rămase pe tablă.
    4. Se elimină Targeturile omorâte de către Hunter și după deplasarea acestora.
    
    Dubla verificare a anihilării Target-urilor, în pașii 2 și 4, îi oferă Hunter-ului
    un avantaj în prinderea lor.
-}

getHunter2Cases :: Position -> Game -> Position
getHunter2Cases(rowIdx, columnIdx) game =
    finalHunter2Pos
    where
        finalHunter2Pos = case checkGateway (rowIdx, columnIdx) (gateways game) of
            -- if it is not a gatewayy
            False -> (rowIdx, columnIdx)
            _ -> (fromJust (findPairedGateway (rowIdx, columnIdx) (gateways game)))

moveHunter :: Direction -> Game -> Game
moveHunter dir game =
    nextGame
        where
            (rowIdx, columnIdx) = (hunter game)
            nextGame = Game (noLines game) (noColumns game) newHunterPos (targets game) (obstacles game) (gateways game)
            newHunterPos = case resultAttemptMove of
                -- need to check whether we sit on a gateway or not
                Nothing -> (rowIdx, columnIdx)
                -- if gateway or if free space just move to That position
                _ -> fromJust (resultAttemptMove)
                where
                    resultAttemptMove = attemptMove pose game
                        where pose = case dir of East -> (rowIdx, columnIdx + 1)
                                                 West -> (rowIdx, columnIdx - 1)
                                                 North -> (rowIdx - 1, columnIdx)
                                                 South -> (rowIdx + 1, columnIdx)
                        

updateTargets :: Game -> Game
updateTargets prevGame =
    newGame
        where
            targetsAlive = filter  (\target -> not (isTargetKilled (hunter prevGame) target)) (targets prevGame)
            gameTargetsAlive = Game (noLines prevGame) (noColumns prevGame) (hunter prevGame) targetsAlive (obstacles prevGame) (gateways prevGame)
            gameTargetsMoved = moveTargets gameTargetsAlive
            targetsAliveAfterMove = filter  (\target -> not (isTargetKilled (hunter prevGame) target)) (targets gameTargetsMoved)
            newGame = Game (noLines gameTargetsMoved) (noColumns gameTargetsMoved) (hunter gameTargetsMoved) targetsAliveAfterMove (obstacles gameTargetsMoved) (gateways gameTargetsMoved)

advanceGameState :: Direction -> Bool -> Game -> Game
advanceGameState dir real game =
    finalGame
        where
            moveHunterGame = moveHunter dir game
            finalGame = case real of False -> moveHunterGame
                                     True  -> updateTargets moveHunterGame

{-
    ***  TODO ***

    Verifică dacă mai există Target-uri pe table de joc.
-}
areTargetsLeft :: Game -> Bool
areTargetsLeft game = not (null (targets game))

{-
    *** BONUS TODO ***

    Comportamentul unui Target de a se deplasa în cerc, în jurul unui Position, având
    o rază fixată.
    Primul parametru, Position, reprezintă centrul cercului.
    Parametrul Int reprezintă raza cercului.
    Puteți testa utilizând terenul circle.txt din directorul terrains, în conjuncție
    cu funcția interactive.
-}
circle :: Position -> Int -> Behavior
circle = undefined

allDirections :: [Direction]
allDirections = [North, South, East, West]

getTargetsKilled :: Position -> [Target] -> [Target]
getTargetsKilled _ [] = []
getTargetsKilled hunter (crtTarget:restTargets) = case isTargetKilled hunter crtTarget of
                                    True -> crtTarget:getTargetsKilled hunter restTargets
                                    False -> getTargetsKilled hunter restTargets

getFirstTargetKilled :: Position -> [Target] -> (Maybe Target)
getFirstTargetKilled pos targets = case getTargetsKilled pos targets of
                                    [] -> Nothing
                                    _ -> Just $ head $ getTargetsKilled pos targets

instance ProblemState Game Direction where
    {-
        *** TODO ***
        
        Generează succesorii stării curente a jocului.
        Utilizați advanceGameState, cu parametrul Bool ales corespunzător.
    -}
    successors game =  zip allDirections $ foldr (\crtDirection games -> (advanceGameState crtDirection False game):games) [] allDirections

    {-
        *** TODO ***
        
        Verifică dacă starea curentă este un în care Hunter-ul poate anihila
        un Target. Puteți alege Target-ul cum doriți, în prezența mai multora.
    -}
    isGoal game = 
        canKill
            where
                canKill = case getFirstTargetKilled (hunter game) (targets game) of
                    Nothing -> False
                    _ -> True

    {-
        *** TODO ***
        
        Euristica euclidiană (vezi hEuclidian mai jos) până la Target-ul ales
        de isGoal.
    -}
    h game = case isGoal game of
                True -> hEuclidean (position $ fromJust $ getFirstTargetKilled (hunter game) (targets game)) (hunter game)
                False -> hEuclidean (position $ head (targets game)) (hunter game)  -- if no target can be killed, take one random

{-
     ** NU MODIFICATI **
-}
hEuclidean :: Position -> Position -> Float
hEuclidean (x1, y1) (x2, y2) = fromIntegral $ ((x1 - x2) ^ pow) + ((y1 - y2) ^ pow)
  where
    pow = 2 :: Int

{-
    *** BONUS ***

    Acesta reprezintă un artificiu necesar pentru testarea bonusului,
    deoarece nu pot exista două instanțe diferite ale aceleiași clase
    pentru același tip.

    OBSERVAȚIE: Testarea bonusului pentru Seach este făcută separat.
-}

newtype BonusGame = BonusGame Game
    deriving (Eq, Ord, Show)

{-
    *** BONUS TODO ***

    Folosind wrapper-ul peste tipul Game de mai sus instanțiați
    ProblemState astfel încât să fie folosită noua euristică. 
-}
instance ProblemState BonusGame Direction where
    {-
        *** BONUS TODO ***

        Pentru a ne asigura că toțî succesorii unei stări sunt de tipul
        BonusGame și folosesc noua euristică trebuie să aplicăm wrapper-ul
        definit mai sus peste toți succesorii unei stări.

        Hint: Puteți să folosiți funcția fmap pe perechi pentru acest lucru.
        https://wiki.haskell.org/Functor
    -}
    successors = undefined

    {-
        *** BONUS TODO ***

        Definiți funcția isGoal pentru BonusGame.

        Hint: Folosiți funcția isGoal deja implementată pentru tipul Game.
    -}
    isGoal = undefined

    {-
        *** BONUS TODO ***

        Definiți o funcție euristică care este capabilă să găsească un drum mai scurt
        comparativ cu cel găsit de euristica implementată pentru Game.

        ATENȚIE: Noua euristică NU trebuie să fie una trivială.

        OBSERVAȚIE: Pentru testare se va folosi fișierul terrains/game-6.txt.
    -}
    h = undefined
