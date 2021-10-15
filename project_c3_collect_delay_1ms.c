

#include "contiki.h"
#include "lib/random.h"
#include "net/rime.h"
#include "net/rime/collect.h"
#include "dev/leds.h"
#include "dev/button-sensor.h"
#include "dev/serial-line.h"

#include "net/netstack.h"

#include <stdio.h>

static struct collect_conn tc;
int temp[4] = {-1,-1,-1,-1};  // 
char arr[100];
int count=0;

/*---------------------------------------------------------------------------*/
PROCESS(example_collect_process, "Test collect process");
AUTOSTART_PROCESSES(&example_collect_process);
/*---------------------------------------------------------------------------*/
static void
recv(const rimeaddr_t *originator, uint8_t seqno, uint8_t hops)
{
  printf("Sink got message from %d.%d, seqno %d, hops %d: len %d '%s'\n",
	 originator->u8[0], originator->u8[1],
	 seqno, hops,
	 packetbuf_datalen(),
	 (char *)packetbuf_dataptr());
}
/*---------------------------------------------------------------------------*/
static const struct collect_callbacks callbacks = { recv };
/*---------------------------------------------------------------------------*/
PROCESS_THREAD(example_collect_process, ev, data)
{

	static struct etimer periodic;
	static struct etimer et;
        static struct etimer delay;
       
	PROCESS_BEGIN();


  collect_open(&tc, 130, COLLECT_ROUTER, &callbacks);
  if(rimeaddr_node_addr.u8[0] == 1 &&
     rimeaddr_node_addr.u8[1] == 0) {
	printf("I am sink\n");
	collect_set_sink(&tc, 1);
  }
  /* Allow some time for the network to settle. */
  etimer_set(&et, 0 * CLOCK_SECOND);
  PROCESS_WAIT_UNTIL(etimer_expired(&et));
  //static int temp[4] = {-1,-1,-1,-1};

etimer_set(&delay, 0.001 * CLOCK_SECOND);
  while(1) {
	
    /* Send a packet every 30 seconds. */
    if(etimer_expired(&periodic)) {
     etimer_set(&periodic, CLOCK_SECOND * 30);
     etimer_set(&et, random_rand() % (CLOCK_SECOND * 30));
    }
	//int temp[4] = {-1,-1,-1,-1};
    PROCESS_WAIT_EVENT();
		/*****************************************************************************/
		
		if(ev == serial_line_event_message && data != NULL) {
				
				int i,j;
				static struct etimer et;
				static struct timer t;
				timer_set(&t, CLOCK_SECOND * 10);
				printf("broadcast message sent\n");
				if(timer_expired(&t)){
				   printf("Timer expired\n");
				}
				
			     printf("I got the message as :-  ");
			    char *line=NULL;
			    
			    line = (char *)data;
			    printf("%s\n",line);
			    printf("Index value is %c\n",line[0]);
				
			     static rimeaddr_t oldparent;
			      const rimeaddr_t *parent;
				int index = (int) line[0]-'0';
				printf("type cast index value %d", index);

				if(index <=3 && index >=0)
				{
					for(i=0;i<=3;i++)
					{
					  printf(" %d ",temp[i]);
					}

					printf("\nindex is %d  ",index);
					printf("if wala block %d\n",temp[index]);
					if(temp[index] == -1)
					{
						temp[index] = index;
						

								printf("Sending if wala\n");
						for(j=0;line[j] != '\0';j++)
						{
							arr[count++] = line[j];
						}

						timer_set(&t, CLOCK_SECOND * 10);
			      			packetbuf_clear();
				
			
                           if(timer_expired(&delay))
                        {  
			packetbuf_set_datalen(sprintf(packetbuf_dataptr(),"aggregated data: %s", arr) + 1);
          etimer_set(&delay, 0.001 * CLOCK_SECOND);			
}			

			     collect_send(&tc, 15);
			      parent = collect_parent(&tc);
			      if(!rimeaddr_cmp(parent, &oldparent)) {
					if(!rimeaddr_cmp(&oldparent, &rimeaddr_null)) {
					  printf("#L %d 0\n", oldparent.u8[0]);
					}
					if(!rimeaddr_cmp(parent, &rimeaddr_null)) {
					  printf("#L %d 1\n", parent->u8[0]);
					}
					rimeaddr_copy(&oldparent, parent);
			      }
						
						 
						
			      			

					}
				}


				else
				{
					printf("Sending else wala \n");
			      	packetbuf_clear();
			      	packetbuf_set_datalen(sprintf(packetbuf_dataptr(),"%s", line) + 1);

			     collect_send(&tc, 15);
			      parent = collect_parent(&tc);
			      if(!rimeaddr_cmp(parent, &oldparent)) {
					if(!rimeaddr_cmp(&oldparent, &rimeaddr_null)) {
					  printf("#L %d 0\n", oldparent.u8[0]);
					}
					if(!rimeaddr_cmp(parent, &rimeaddr_null)) {
					  printf("#L %d 1\n", parent->u8[0]);
					}
					rimeaddr_copy(&oldparent, parent);
			      }
				}



			 	

			
		}

}
		
  PROCESS_END();
}
/*---------------------------------------------------------------------------*/
