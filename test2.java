
import java.io.*;
import java.nio.charset.Charset;
import java.nio.file.Files;
import java.nio.file.Path;
import java.nio.file.Paths;
import java.util.*;
import java.util.PriorityQueue;

abstract class HuffmanTree implements Comparable<HuffmanTree> {
    public final int frequency; // the frequency of this tree
    public HuffmanTree(int freq) { 
     frequency = freq; 
    }

    // compares on the frequency
    public int compareTo(HuffmanTree tree) {
        return frequency - tree.frequency;
    }
}

class HuffmanLeaf extends HuffmanTree {
    public final char charcter; // the character this leaf represents

    public HuffmanLeaf(int freq, char val) {
        super(freq);
        charcter = val;
    }
}

class HuffmanNode extends HuffmanTree {
    public final HuffmanTree left, right; // subtrees

    public HuffmanNode(HuffmanTree l, HuffmanTree r) {
        super(l.frequency + r.frequency);
        left = l;
        right = r;
    }
}

public class HuffmanCode{
      Map<Character, Integer> charFre = new TreeMap<Character, Integer>(); // map to store char and frequency
 
 /* This function iterates through the buffer and reads the char and their frequency of occurance
  * and create map to store character as key and value as frequency  and stores the respective values
  *  Input : Reader
  *  Output: void
  */
 public void GetFrequency(BufferedReader reader) throws IOException{
  int c = 0;
  // read characters from the file
  while((c = reader.read()) != -1) {
   char ch = (char) c;
   if(Character.isAlphabetic(ch) || Character.isDigit(ch) ){ // check for digits and characters
    // adding char to map
    if(charFre.containsKey(ch)){
     int value = charFre.get(ch);
     charFre.put(ch, value+1);
    }else{
     charFre.put(ch,1);
    }
   }
  }// end of while  
  //System.out.println(charFre);
 }
 
  
  
  public static void printCodes(HuffmanTree tree, StringBuffer prefix) throws IOException {
         if (tree instanceof HuffmanLeaf) {
             HuffmanLeaf leaf = (HuffmanLeaf)tree;

             // print out character, frequency, and code for this leaf (which is just the prefix)
           System.out.println(leaf.charcter + "\t" + leaf.frequency + "\t" + prefix);
             
         } else if (tree instanceof HuffmanNode) {
             HuffmanNode node = (HuffmanNode)tree;

             // traverse left
             prefix.append('0');
             printCodes(node.left, prefix);
             prefix.deleteCharAt(prefix.length()-1);

             // traverse right
             prefix.append('1');
             printCodes(node.right, prefix);
             prefix.deleteCharAt(prefix.length()-1);
         }
     }
 public static void main(String[] args) throws IOException {
  HuffmanCode hc = new HuffmanCode();
  String folderPath = "C:/eclipse/Programmes/Datastructure/"; // path of the input file
  Path path = Paths.get(folderPath, "infix.dat"); // get file
  Charset charset = Charset.forName("UTF-8");
  // Read the file at specified location
  BufferedReader reader = Files.newBufferedReader(path , charset); // Read file into the buffer
  hc.GetFrequency(reader);
  hc.buildTree();
  HuffmanTree tree = hc.buildTree();
     // PrintStream out = new PrintStream(new File("C:/eclipse/Programmes/Algorithm/output.txt"));
     // System.setOut(out);
        // print out results
    System.out.println("SYMBOL\tFREQ\tHUFFMAN CODE");
        printCodes(tree, new StringBuffer());
 }
 
 public HuffmanTree buildTree() {
         PriorityQueue<HuffmanTree> trees = new PriorityQueue<HuffmanTree>();
         // initially, we have a forest of leaves
         // one for each non-empty character
         if(!charFre.isEmpty()){     // check whether hashmap is empty
     Set keys = charFre.keySet();
     Iterator i = keys.iterator();
     while(i.hasNext()){
      char key = (char) i.next();
      int value = (int) charFre.get(key);
                 trees.offer(new HuffmanLeaf(value, key));
     }
         }   
       
         // loop until there is only one tree left
         while (trees.size() > 1) {
             // two trees with least frequency
             HuffmanTree t1 = trees.poll();
             HuffmanTree t2 = trees.poll();

             // put into new node and re-insert into queue
             trees.offer(new HuffmanNode(t1,t2));
         }
         return trees.poll();
     }
}
