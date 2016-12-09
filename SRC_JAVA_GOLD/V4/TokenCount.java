import java.util.Set;
import java.util.ArrayList;
import java.util.Map;
import java.util.Map.Entry;
import java.util.Collections;
import java.util.Comparator;
import java.util.concurrent.ArrayBlockingQueue;
import java.util.concurrent.ExecutorService;
import java.util.concurrent.Executors;
import java.util.concurrent.TimeUnit;
import java.util.concurrent.ConcurrentHashMap;

public class TokenCount
{
    private static final ConcurrentHashMap<String, Integer> tokenFreq = new ConcurrentHashMap<String, Integer>();
	private static final ArrayBlockingQueue<Page> Q = new ArrayBlockingQueue<Page>(100);
	
    public static void main(String[] args) throws Exception
	{
		if (args.length != 2) 
		{
			System.out.println("usage: java TokenCount number-of-pages XML-file");
			System.exit(0);
		}
		Integer numPages = Integer.parseInt(args[0]);
		Integer numProcs = Runtime.getRuntime().availableProcessors();
		Integer i;
		final long before, after;
		
		// consumer pool and producer threads
		Thread prod = new Thread(new Producer(Q,numPages,args[1]));
		ExecutorService consPool = Executors.newFixedThreadPool(numProcs - 1);
		//Thread cons = new Thread(new Consumer(Q, tokenFreq));

		// print number of available processors
		System.out.println(numProcs + " available processors");
		
		// begin timed code ...
		before = System.nanoTime();
		
		// starting consumer and producer threads
		prod.start();
		for(i=0;i < numProcs;i++)
		{
			Runnable cons = new Consumer(Q,tokenFreq);
			consPool.execute(cons);
		}
		consPool.shutdown();
		try 
		{
			consPool.awaitTermination(Long.MAX_VALUE, TimeUnit.NANOSECONDS);
		} 
		catch (InterruptedException e) {}
		
		// ... end  timed code
		after = System.nanoTime();

		System.out.println("Time to process " + numPages + " pages = " + (after - before)/1000000 + " milliseconds");

		// sort tokenFreq by value & print top 30 most common tokens
		Set<Entry<String, Integer>> entries = tokenFreq.entrySet();
		ArrayList<Entry<String, Integer>> list = new ArrayList<Entry<String, Integer>>(entries);
		Collections.sort(list, new Comparator<Map.Entry<String, 
			Integer>>()
			{
				public int compare(Map.Entry<String, Integer> obj1, Map.Entry<String, Integer> obj2)
				{
					return (obj2.getValue()).compareTo(obj1.getValue());
				}
			}
		);
		
		for(i=0; i<30; i++)
			System.out.println(list.get(i).getKey() + " appears " + list.get(i).getValue() + " times");
	}
}

class Producer implements Runnable
{
	private ArrayBlockingQueue<Page> Q;
	Integer numPages;
	String arg;
	
	public Producer(ArrayBlockingQueue<Page> Q, Integer numPages, String arg)
	{
		this.Q = Q;
		this.numPages = numPages;
		this.arg = arg;
	}
	
	public void run()
	{
		Iterable<Page> allPages = new Pages(numPages, arg);
		
		// feed pages to the consumer
		for (Page pg: allPages) 
		{
			try
			{
				Q.put(pg);
			}
			catch(Exception e){}	
		}
		try
		{
			// send the thread kill message
			Q.put(new PoisonPill());
		}
		catch(Exception e){}
	}
}

class Consumer implements Runnable
{
	private ArrayBlockingQueue<Page> Q;
	private static ConcurrentHashMap<String, Integer> tokenFreq;
	
	public Consumer(ArrayBlockingQueue<Page> Q, ConcurrentHashMap<String, Integer> tokenFreq)
	{
		this.Q = Q;
		this.tokenFreq = tokenFreq;
		
	}
	
	public void run()
	{
		ConcurrentHashMap<String, Integer> threadMap = new ConcurrentHashMap<String, Integer>();
		Iterable<String> allTokens;
		
		// read pages from the producer
		while(true)
		{
			try
			{
				Page pg = Q.take();
				if(pg.isPoisonPill()) 
				{
					Q.put(pg);
					mergeMaps(threadMap);
					return;
				}
				allTokens = new Words(pg.getText());
				for (String s: allTokens)
					countToken(threadMap,s);
			}
			catch(Exception e){}
		}
	}
	
	private static void countToken(ConcurrentHashMap<String, Integer> threadMap, String tok)
	{	
		Integer currentCount = threadMap.get(tok);
		
		// insert or update the token with the number of occurrences
		if (currentCount == null) threadMap.put(tok, 1);
		else threadMap.put(tok, currentCount + 1);
    }
	
	public static void mergeMaps(ConcurrentHashMap<String, Integer> threadMap)
	{
		Integer currentCount;
		
		// for all the keys, get their amount and insert them into the the table
		for(String elems: threadMap.keySet())
		{
			currentCount = tokenFreq.get(elems);
			
			if (currentCount == null) tokenFreq.put(elems, threadMap.get(elems));
			else tokenFreq.put(elems, currentCount + threadMap.get(elems));
		}
	}
}