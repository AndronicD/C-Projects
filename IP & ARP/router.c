#include "queue.h"
#include "skel.h"
#include<stdlib.h>
#include<stdio.h>

#define MAX_TTL 64


int comparator(const void* a, const void* b){
	struct route_table_entry *first = (struct route_table_entry*) a;
	struct route_table_entry *second = (struct route_table_entry*) b;
	if(ntohl(first->prefix) == ntohl(second->prefix)){
		return (int)(first->mask - second->mask);
	}
	return (int)(first->prefix - second->prefix);
}

struct route_table_entry* binary_search(__u32 dest, int l, int r, struct route_table_entry *rtable){
	int mid = 0;
	int left = l;
	int right = r;
	int indexFin = -1;
	while(left < right){
		mid = left + (right - left)/2;
		if((rtable[mid].mask & dest) == rtable[mid].prefix){
			uint32_t prefix = rtable[mid].prefix;
			while(rtable[mid].prefix == prefix){
				mid++;
			}
			indexFin = mid;
			left = mid + 1;
		}
		else if(ntohl(dest) > ntohl(rtable[mid].prefix)){
			left = mid + 1;
		}
		else if(ntohl(dest) < ntohl(rtable[mid].prefix)){
			right = mid - 1;
		}
	}
	if(indexFin == -1){
		return NULL;
	}
	return &rtable[indexFin];
}

struct arp_entry* get_arp_entry(__u32 ip, struct arp_entry *arptable){
	for(int i = 0; i < 6; i++){
		if(ntohl(arptable[i].ip) == ntohl(ip)){
			return &arptable[i];
		}
	}
	return NULL;
}

void send_icmp_packet(uint8_t* old_payload, uint32_t daddr, uint32_t saddr, uint8_t *sha, uint8_t *dha, uint8_t type, uint8_t code, int interface, int id, int seq, int error){
	struct ether_header eth_hdr;
	struct iphdr ip_hdr;
	struct icmphdr icmp_hdr;
	packet packet_to_send;
	void* pointer;

	//Ethernet Header
	memcpy(&eth_hdr.ether_dhost, dha, 6);
	memcpy(&eth_hdr.ether_shost, sha, 6);
	eth_hdr.ether_type = htons(ETHERTYPE_IP);

	//IP Header
	ip_hdr.version = 4;
	ip_hdr.ihl = 5;
	ip_hdr.tos = 0;
	ip_hdr.frag_off = 0;
	ip_hdr.id = htons(1);
	ip_hdr.tot_len = htons(sizeof(struct iphdr) + sizeof(struct icmphdr));
	ip_hdr.protocol = IPPROTO_ICMP;
	ip_hdr.ttl = MAX_TTL;
	ip_hdr.check = 0;
	ip_hdr.daddr = daddr;
	ip_hdr.saddr = saddr;
	ip_hdr.check = ip_checksum((uint8_t*)&ip_hdr, sizeof(struct iphdr));

	//ICMP Header
	icmp_hdr.type = type;
	icmp_hdr.code = code;
	icmp_hdr.checksum = 0;

	if(error == 0){
		icmp_hdr.un.echo.id = id;
		icmp_hdr.un.echo.sequence = seq;
	}
	icmp_hdr.checksum = icmp_checksum((uint16_t*)&icmp_hdr, sizeof(struct icmphdr));

	pointer = packet_to_send.payload;
	memcpy(pointer, &eth_hdr, sizeof(struct ether_header));
	pointer += sizeof(struct ether_header);
	memcpy(pointer, &ip_hdr, sizeof(struct iphdr));
	pointer += sizeof(struct iphdr);
	memcpy(pointer, &icmp_hdr, sizeof(struct icmphdr));
	pointer += sizeof(struct icmphdr);
	memcpy(pointer, old_payload, 64);
	pointer += 64;

	packet_to_send.len = sizeof(struct ether_header) + sizeof(struct iphdr) + sizeof(struct icmphdr) + 64;
	packet_to_send.interface = interface;
	send_packet(&packet_to_send);
}

struct route_table_entry *get_best_route_lin(__u32 dest_ip, struct route_table_entry* rtable, int rtable_len) {
    size_t idx = -1;	

    for (size_t i = 0; i < rtable_len; i++) {
        if ((dest_ip & rtable[i].mask) == rtable[i].prefix) {
	    	if (idx == -1) idx = i;
	    	else if (ntohl(rtable[idx].mask) < ntohl(rtable[i].mask)) idx = i;
	    	else if (rtable[idx].mask == rtable[i].mask) idx = i;
		}
	}
    
    if (idx == -1)
        return NULL;

    else
        return &rtable[idx];
}

int main(int argc, char *argv[])
{
	packet m;
	int rc;

	
	struct route_table_entry *rtable;
	int rtable_len;
	struct arp_entry *arptable;
	int arptable_len;	
	
	rtable = malloc(sizeof(struct route_table_entry) * 100000);
	DIE(rtable == NULL, "memory");

	rtable_len = read_rtable(argv[1], rtable);
	qsort(rtable, rtable_len, sizeof(struct route_table_entry), comparator);

	arptable = malloc(sizeof(struct arp_entry) * 6);
	parse_arp_table("arp_table.txt", arptable);
	arptable_len = 6;

	// Do not modify this line
	init(argc - 2, argv + 2);

	while (1) {
		rc = get_packet(&m); 
		DIE(rc < 0, "get_packet");
		/* TODO */
		struct ether_header *eth_hdr = (struct ether_header *) m.payload;
		struct iphdr *ip_hdr = (struct iphdr *) (m.payload + sizeof(struct ether_header));
		struct icmphdr *icmp_hdr = (struct icmphdr *) (m.payload + sizeof(struct ether_header) + sizeof(struct iphdr)); 
		
		if(ntohs(eth_hdr->ether_type) == ETHERTYPE_IP){
			if(ip_checksum((uint8_t*)ip_hdr, sizeof(struct iphdr)) != 0){
				continue;
			}
		
			if(ip_hdr->daddr == inet_addr(get_interface_ip(m.interface))){
				if(icmp_hdr->type == 8){
					send_icmp_packet((uint8_t*)m.payload, ip_hdr->saddr, ip_hdr->daddr, eth_hdr->ether_dhost, eth_hdr->ether_shost, 0, 0, m.interface, icmp_hdr->un.echo.id, icmp_hdr->un.echo.sequence, 0);	
					continue;
				}
				continue;
			}
		
			ip_hdr->check = 0;
			ip_hdr->check = ip_checksum((uint8_t*)ip_hdr, sizeof(struct iphdr));
			
			if(ip_hdr->ttl <= 1){
				send_icmp_packet((uint32_t*)m.payload, ip_hdr->saddr, ip_hdr->daddr, eth_hdr->ether_dhost, eth_hdr->ether_shost, ICMP_TIME_EXCEEDED, ICMP_EXC_TTL, m.interface, 0, 0, 1);
				continue;
			}
			
			ip_hdr->ttl--;
			ip_hdr->check = 0;
			ip_hdr->check = ip_checksum((uint8_t*)ip_hdr, sizeof(struct iphdr));
			
			struct route_table_entry *ip_route = get_best_route_lin(ip_hdr->daddr, rtable, rtable_len);
			//struct route_table_entry *ip_route2 = binary_search(ip_hdr->daddr, 0, rtable_len - 1, rtable);
			//printf("prefix:%d next_hop:%d interface: %d mask: %d\n", ip_route->prefix, ip_route->next_hop, ip_route->interface, (ip_route->mask));
			//printf("prefix:%d next_hop:%d interface: %d mask: %d\n", ip_route2->prefix, ip_route2->next_hop, ip_route2->interface, (ip_route2->mask));

			if(ip_route == NULL){
				send_icmp_packet((uint8_t*)m.payload ,ip_hdr->saddr, ip_hdr->daddr, eth_hdr->ether_dhost, eth_hdr->ether_shost, ICMP_DEST_UNREACH, 0, m.interface, 0, 0, 1);
				continue;
			}
			
			struct arp_entry *eth_route = get_arp_entry((__u32)ip_route->next_hop, arptable);
			get_interface_mac(ip_route->interface, eth_hdr->ether_shost);
			memcpy(eth_hdr->ether_dhost, eth_route->mac, sizeof(eth_route->mac));
			m.interface = ip_route->interface;
			send_packet(&m);
		}
	}
	free(rtable);
	free(arptable);
}
