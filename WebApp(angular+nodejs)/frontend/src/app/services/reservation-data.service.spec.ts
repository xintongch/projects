import { TestBed } from '@angular/core/testing';

import { ReservationDataService } from './reservation-data.service';

describe('ReservationDataService', () => {
  let service: ReservationDataService;

  beforeEach(() => {
    TestBed.configureTestingModule({});
    service = TestBed.inject(ReservationDataService);
  });

  it('should be created', () => {
    expect(service).toBeTruthy();
  });
});
