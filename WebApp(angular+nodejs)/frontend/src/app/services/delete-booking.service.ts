import { Injectable } from '@angular/core';
import { ClickSubmitReservationService } from './click-submit-reservation.service';
// import { ReservationDataService } from './reservation-data.service';

@Injectable({
  providedIn: 'root',
})
export class DeleteBookingService {
  constructor(
    private clickSubmitReservationService: ClickSubmitReservationService,
    // private reservationDataService:ReservationDataService
  ) {}
  deleteBooking(id: any) {
    this.clickSubmitReservationService.deleteBooking(id);
    $('#tr' + id).remove();
    let len = $('#b-table tbody').children().length;
    for (let i = id + 1; i <= len+1; i++) {
      let newi = i - 1;
      $('#tr' + i + ' > #booking-record-index').html('' + newi);
      $('#tr' + i).attr('id', 'tr' + newi);
      $('#img' + i).attr('id', 'img' + newi);
    }
    if ($('#b-table tbody').children().length == 0) {
      $('#no-booking').attr('style', 'display:inline-block');
      $('#br-container').attr('style', 'display:none');
    }
  }
}
