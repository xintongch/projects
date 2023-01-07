import { TestBed } from '@angular/core/testing';

import { ClickSubmitReservationService } from './click-submit-reservation.service';

describe('ClickSubmitReservationService', () => {
  let service: ClickSubmitReservationService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ClickSubmitReservationService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
