import Data.List
import Data.Char
import Data.Maybe

type FreqTable = [(Char, Int)]

-- a
rareChars :: FreqTable -> Int -> [Char]
rareChars ((crtChar, crtFrequency):xs) n
    | crtFrequency < n = crtChar:(rareChars xs n)
    | otherwise = rareChars xs n
rareChars _ _ = []

-- b
insChar :: Char -> FreqTable -> FreqTable
insChar char freqTable = case (filter (\x -> (fst x == char)) freqTable) of
    [] -> (char, 1):freqTable
    [(crtChar, freqCoresspondig)] -> (crtChar, freqCoresspondig + 1):(filter (\x -> (fst x /= char)) freqTable)
    
member :: (Eq a) => a -> [a] -> Bool
member x [] = False
member x (y:ys)
    | x==y = True
    | otherwise = member x ys

textToStr :: String -> String
textToStr text = 
    let wordsInText = words text
    in (foldr (\crtWord crtAcc -> crtWord ++ crtAcc) "" (reverse (foldl (\crtAcc crtWord -> if (member crtWord crtAcc) then crtAcc else crtWord:crtAcc) [] wordsInText)))
   
textToTable :: String -> FreqTable
textToTable text = 
    let textString = textToStr text
    in (foldr (\crtChar crtAcc -> insChar crtChar crtAcc) [] textString)

data Val = I Int | C Char | P (Int, Char) deriving (Show, Eq)
data MList = M [Val] deriving Show

history = [("MN", 4), ("AA", 10), ("PP", 10)]

get_type hist
    | nota < 5 = "repetent"
    | (nota >= 5) && (nota < 10) = "bun"
    | nota == 10 = "perfect"
    where
        nota = snd (head (filter ((== "PP") . fst ) hist))

isI :: Val -> Bool
isI (I _) = True
isI _ = False

isC :: Val -> Bool
isC (C _) = True
isC _ = False

isP :: Val -> Bool
isP (P _) = True
isP _ = False

extractChar:: Val -> Maybe Char
extractChar (C a) = Just a
extractChar _ = Nothing


filter' :: Char -> MList -> MList
filter' character (M my_list) = case character of
    'i' -> M $ filter (\x -> isI x) my_list
    'c' -> M $ filter (\x -> isC x) my_list
    'p' -> M $ filter (\x -> isP x) my_list 
    
var1 = I 2
var2 = C 'a'
var3 = C 'A'
var4 = P (2, 'a')
list = [ var2, var3]
mlist = M list 
 
conv :: MList -> Maybe [Char]
conv (M my_list) = 
    if ( length_characters == length_my_list) then jst_characters else Nothing
    where
    characters = filter isC my_list
    length_characters = length $ characters
    length_my_list = length my_list
    jst_characters = Just $ map (fromJust . extractChar) characters
    --jst_characters = Just $ map snd characters
              --      characters = filter isC my_list
              --      length_characters = length $ characters
     
--    result
--    where
--        result = case length_characters of
--            6 -> 2
--            _ -> Nothing
--                where
--                characters = filter isC my_list
--                length_characters = length $ characters
--                length_my_list = length my_list
--                jst_characters = Just characters
            

--conv (M ((C c): rest)) = 
--    case (conv (M rest)) of
--        Just l -> Just (c:l)
--        Nothing -> Nothing
--conv(M []) = Just []
--conv _ = Nothing

data Queue a = Queue ([a], [a]) deriving Show

isEmpty :: Queue a -> Bool
isEmpty (Queue (outStack, inStack)) = (null outStack) && (null inStack)

coada_mea = Queue ([], [1, 2, 3])

top :: Queue a -> Maybe a
top (Queue (outStack, inStack)) =
    if (null outStack)
        then if (null inStack)
            then Nothing
            else Just (last inStack)
        else Just (head outStack)
        
del :: (Eq a) => Queue a -> Maybe (Queue a)
del (Queue (outStack, inStack)) =
    if (null outStack)
        then if (null inStack)
            then Nothing
            else Just (Queue ((reverse (delete (last inStack) (reverse inStack))), []))
        else Just (Queue ((delete (head outStack) outStack), inStack))
        
insertQueue :: (Queue a) -> a -> (Queue a)    
insertQueue (Queue (outStack, inStack)) elem = 
    (Queue (outStack, elem:inStack))

isPrime k = if k > 1 then null [ x | x <- [2..k - 1], k `mod` x == 0] else False

decompose n = head [(x, y) | x <- [1..n], isPrime x, let y = n - x, isPrime y]