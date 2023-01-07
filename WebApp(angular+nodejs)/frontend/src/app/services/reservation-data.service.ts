import { Injectable } from '@angular/core';

@Injectable({
  providedIn: 'root'
})
export class ReservationDataService {
  reservationData:any={};

  constructor() { }
  addReservationData(id:any,index:any){

    this.reservationData=JSON.parse(window.localStorage.getItem('reservationData') || '{}')
    console.log('add1, reD=',this.reservationData)
    this.reservationData[id]=index;
    console.log('add2, reD=',this.reservationData)
    window.localStorage.setItem('reservationData',JSON.stringify(this.reservationData))
  }
  removeReservationData(id:any){
    this.reservationData=JSON.parse(window.localStorage.getItem('reservationData') || '{}')
    console.log('remove1, reD=',this.reservationData)

    delete this.reservationData[id];
    console.log('remove2, reD=',this.reservationData)
    window.localStorage.setItem('reservationData',JSON.stringify(this.reservationData))

  }
  checkReservationData(id:any){
    this.reservationData=JSON.parse(window.localStorage.getItem('reservationData') || '{}')
    console.log('check, reD=',this.reservationData)
    if(this.reservationData.hasOwnProperty(id)){
      return true;
    }
    return false;
  }
  getReservationIndex(id:any){
    this.reservationData=JSON.parse(window.localStorage.getItem('reservationData') || '{}')
    console.log('get, reD=',this.reservationData)
    return this.reservationData[id]
  }
}
