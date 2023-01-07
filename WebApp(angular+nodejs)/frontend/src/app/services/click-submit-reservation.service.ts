import { Injectable } from '@angular/core';
import { ReservationDataService } from './reservation-data.service';

@Injectable({
  providedIn: 'root',
})
export class ClickSubmitReservationService {
  bookings:any;
  email:any;
  date:any;
  time:any;
  businessName:any;
  bookingIndex=0;
  deleteImageURL="assets/deleteImage.png";
  bookings_table_html_text="";

  constructor(
    private reservationDataService: ReservationDataService,
  ) {}

  createBookingsTableHtmlText(){
    this.bookings_table_html_text="";
    this.bookingIndex=this.bookings.length;
    for(let i=1;i<=this.bookingIndex;i++){
      this.bookings_table_html_text+="<tr class='hover-gray' id='tr"+i+"' style='height:20px;border-bottom: 1.5px solid rgb(236, 234, 234);'><td id='booking-record-index' class='font-weight-bold'>"+i+"</td><td class=''>"+this.bookings[i-1].businessName+"</td><td>"+this.bookings[i-1].date+"</td><td>"+this.bookings[i-1].time+"</td><td>"+this.bookings[i-1].email+"</td><td><img src='"+this.deleteImageURL+"' id='img"+i+"' style='width:17px;height:17px;object-fit:fill;cursor:pointer'></td></tr>"
    }

    window.localStorage.setItem('bookings_table_html_text',this.bookings_table_html_text);
  }



  deleteBooking(id:any){
    console.log('delete booking id/index=',id)
    this.bookings=JSON.parse(window.localStorage.getItem('bookings') || '[]')
    console.log('bookings1=',this.bookings)
    this.reservationDataService.removeReservationData(this.bookings[id-1].id)
    this.bookings.splice(id-1,1)
    console.log('bookings2=',this.bookings)
    window.localStorage.setItem('bookings',JSON.stringify(this.bookings));
    let len=this.bookings.length;
    console.log('bookings len=',len);
    for(let i=id-1;i<len;i++){
      console.log('i=',i);
      this.reservationDataService.addReservationData(this.bookings[i].id,i+1)
    }
    this.createBookingsTableHtmlText();
  }



  updateBookingPageHTMLText(){
    this.bookings_table_html_text+="<tr class='hover-gray border-bottom' id='tr"+this.bookingIndex+"' style='height:20px;border-bottom: 1.5px solid rgb(236, 234, 234);'><th scope='row' id='booking-record-index' class='font-weight-bold'>"+this.bookingIndex+"</th><td>"+this.businessName+"</td><td>"+this.date+"</td><td>"+this.time+"</td><td>"+this.email+"</td><td><img src='"+this.deleteImageURL+"' id='img"+this.bookingIndex+"' style='width:17px;height:17px;object-fit:fill;cursor:pointer'></td></tr>"
  }



  reserve(businessName:any,id:string) {

    this.bookings.push({"businessName":businessName,"email":this.email,"date":this.date,"time":this.time,"id":id});
    this.bookingIndex=this.bookings.length;
    this.reservationDataService.addReservationData(id,this.bookingIndex);
    this.updateBookingPageHTMLText();
  }


  
  submitReservation(businessName:any,id:any) {
    this.bookings=JSON.parse(window.localStorage.getItem('bookings') || '[]')
    this.bookingIndex=this.bookings.length==0?0:this.bookings.length-1;
    this.bookings_table_html_text=window.localStorage.bookings_table_html_text;

    this.email=$("#email").val();
    this.date=$("#date").val();
    this.time=$("#hour").val()+':'+$("#minute").val();
    this.businessName=businessName;
    if (this.email == '') {
      (<HTMLInputElement>(
        document.getElementById('reservation-form')
      )).reportValidity();
      return;
    }
    if (this.date == '') {
      (<HTMLInputElement>(
        document.getElementById('reservation-form')
      )).reportValidity();
      return;
    }
    if (this.time == '') {
      (<HTMLInputElement>(
        document.getElementById('reservation-form')
      )).reportValidity();
      return;
    }
    this.reserve(businessName,id);
    window.localStorage.setItem('bookings',JSON.stringify(this.bookings));
    window.localStorage.setItem('bookings_table_html_text',this.bookings_table_html_text);
  }

}
