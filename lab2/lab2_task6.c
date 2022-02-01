#include <math.h>
#include <string.h>
#include <stdio.h>


double split_bill ( double base_amount , double tax_rate , double tip_rate , int num_people ) {

	double final_bill;
	final_bill = base_amount * (1+tax_rate) * (1+tip_rate);
	double indv_bill = final_bill / num_people;
	int temp = indv_bill * 100;
	if (indv_bill * 100 > temp){
		temp++;
	}
	return (double) temp/100;
}


double adjust_price ( double original_price ) {

	double adjusted_price;
	adjusted_price = 10 * pow(original_price,0.5);
	return adjusted_price;
}


int sandy_eats ( char menu_item [] ){

	if (strchr(menu_item,'j')!=NULL || strchr(menu_item,'J')!=NULL){
		return 0;
	}
	if (strchr(menu_item,'k')!=NULL || strchr(menu_item,'K')!=NULL){
		return 0;
	}
	if (strchr(menu_item,'l')!=NULL || strchr(menu_item,'L')!=NULL){
		return 0;
	}
	if (strstr(menu_item,"Fish")!=NULL || strstr(menu_item,"fish")!=NULL){
		return 0;
	}
	if (strlen(menu_item)%2 != 0){
		return 0;
	}
	return 1;
}


void imagine_fish ( char thing [] ){

	strcat(thing,"fish");
}